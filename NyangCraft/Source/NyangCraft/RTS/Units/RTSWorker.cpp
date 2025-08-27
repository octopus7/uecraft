#include "RTS/Units/RTSWorker.h"
#include "RTS/Resources/RTSResource_Mineral.h"
#include "RTS/Buildings/RTSBuilding_CommandCenter.h"
#include "RTS/Systems/RTSGameMode.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "NyangCraft.h"
#include <cfloat>
#include "Components/TextRenderComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

ARTSWorker::ARTSWorker()
{
    StatusText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("StatusText"));
    if (StatusText)
    {
        StatusText->SetupAttachment(RootComponent);
        StatusText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
        StatusText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
        StatusText->SetWorldSize(36.f);
        StatusText->SetTextRenderColor(FColor::Cyan);
        StatusText->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
        StatusText->SetHiddenInGame(false);
        StatusText->SetMobility(EComponentMobility::Movable);
    }
    PrimaryActorTick.bCanEverTick = true;
    UpdateStatusText();
}

void ARTSWorker::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    FaceCamera();
}

void ARTSWorker::IssueGatherOrder(ARTSResource_Mineral* InResource)
{
    // Reset any previous work
    FinishAllTasks();

    if (!InResource)
    {
        Phase = EWorkerPhase::Idle;
        UpdateStatusText();
        return;
    }

    TargetResource = InResource;
    Phase = EWorkerPhase::MoveToResource;
    MoveToResource();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Yellow,
            FString::Printf(TEXT("[RTS] %s 채집 명령 → %s"), *GetName(), *InResource->GetName()));
    }
    UE_LOG(LogNyangCraft, Log, TEXT("[RTS] %s issued gather order to %s"), *GetName(), *InResource->GetName());
    UpdateStatusText();
}

void ARTSWorker::MoveToResource()
{
    if (!TargetResource.IsValid())
    {
        FinishAllTasks();
        return;
    }
    Phase = EWorkerPhase::MoveToResource;
    UpdateStatusText();
    MoveToActor(TargetResource.Get(), AcceptanceRadius);
}

void ARTSWorker::MoveToDropoff()
{
    DropoffTarget = FindNearestCommandCenter();
    if (!DropoffTarget.IsValid())
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red,
                TEXT("[RTS] 반납 대상 없음: 작업 중단"));
        }
        UE_LOG(LogNyangCraft, Warning, TEXT("[RTS] %s no dropoff found"), *GetName());
        FinishAllTasks();
        return;
    }
    Phase = EWorkerPhase::MoveToDropoff;
    UpdateStatusText();
    MoveToActor(DropoffTarget.Get(), AcceptanceRadius);
    UpdateStatusText();
}

void ARTSWorker::StartGatherTick()
{
    Phase = EWorkerPhase::Gather;
    UpdateStatusText();
    if (GatherDuration <= 0.f)
    {
        DoGatherTick();
        return;
    }
    GetWorldTimerManager().SetTimer(GatherTimerHandle, this, &ARTSWorker::DoGatherTick, GatherDuration, false);
}

void ARTSWorker::DoGatherTick()
{
    if (!TargetResource.IsValid())
    {
        FinishAllTasks();
        return;
    }

    const int32 Space = FMath::Max(0, CarryCapacity - Carried);
    const int32 ToHarvest = FMath::Min(Space, GatherPerTick);
    int32 Harvested = 0;
    if (ToHarvest > 0)
    {
        Harvested = TargetResource->Harvest(ToHarvest);
        Carried += Harvested;
    }
    if (GEngine && Harvested > 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,
            FString::Printf(TEXT("[RTS] %s 채집 +%d (%d/%d)"), *GetName(), Harvested, Carried, CarryCapacity));
    }

    const bool bFull = (Carried >= CarryCapacity);
    const bool bDepleted = !TargetResource.IsValid() || TargetResource->IsDepleted();

    if (bFull || bDepleted)
    {
        MoveToDropoff();
    }
    else
    {
        StartGatherTick();
    }
}

void ARTSWorker::StartDeposit()
{
    Phase = EWorkerPhase::Deposit;
    UpdateStatusText();
    if (DepositDuration <= 0.f)
    {
        DoDeposit();
        return;
    }
    GetWorldTimerManager().SetTimer(DepositTimerHandle, this, &ARTSWorker::DoDeposit, DepositDuration, false);
}

void ARTSWorker::DoDeposit()
{
    if (Carried > 0)
    {
        if (UWorld* World = GetWorld())
        {
            if (ARTSGameMode* GM = World->GetAuthGameMode<ARTSGameMode>())
            {
                GM->AddMinerals(Carried);
            }
        }
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Silver,
                FString::Printf(TEXT("[RTS] %s 반납 +%d"), *GetName(), Carried));
        }
        Carried = 0;
    }
    UpdateStatusText();

    // Continue if resource remains
    if (TargetResource.IsValid() && !TargetResource->IsDepleted())
    {
        Phase = EWorkerPhase::MoveToResource;
        MoveToResource();
    }
    else
    {
        FinishAllTasks();
    }
}

void ARTSWorker::FinishAllTasks()
{
    GetWorldTimerManager().ClearTimer(GatherTimerHandle);
    GetWorldTimerManager().ClearTimer(DepositTimerHandle);
    Phase = EWorkerPhase::Idle;
    UpdateStatusText();
}

ARTSBuilding_CommandCenter* ARTSWorker::FindNearestCommandCenter() const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    ARTSBuilding_CommandCenter* Best = nullptr;
    float BestDist = FLT_MAX;
    for (TActorIterator<ARTSBuilding_CommandCenter> It(World); It; ++It)
    {
        ARTSBuilding_CommandCenter* CC = *It;
        const float D = FVector::DistSquared(GetActorLocation(), CC->GetActorLocation());
        if (D < BestDist)
        {
            BestDist = D;
            Best = CC;
        }
    }
    return Best;
}

bool ARTSWorker::MoveToActor(AActor* Actor, float Acceptance)
{
    if (!Actor) return false;
    if (AAIController* AI = Cast<AAIController>(GetController()))
    {
        // Debug: target bounds (pink or red if very small) and acceptance radius (green)
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
        if (UWorld* World = GetWorld())
        {
            float BoundsR = 0.f;
            if (const UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
            {
                BoundsR = Prim->Bounds.SphereRadius;
            }
            const bool bTiny = BoundsR < 10.f;
            const float EffectiveR = bTiny ? 100.f : BoundsR;
            const FColor Pink(255, 105, 180);
            const FColor BoundsColor = bTiny ? FColor::Red : Pink;
            DrawDebugSphere(World, Actor->GetActorLocation(), EffectiveR, 24, BoundsColor, false, 1.0f, 0, 1.5f);
            // Worker-centered acceptance radius (green)
            DrawDebugSphere(World, GetActorLocation(), Acceptance, 24, FColor::Green, false, 1.0f, 0, 0.75f);
        }
#endif
        // Choose a reachable point near the actor and move to that location
        FVector Reachable;
        const bool bHaveReachable = FindReachablePointNear(Actor->GetActorLocation(), Reachable, 500.f);
        EPathFollowingRequestResult::Type R = EPathFollowingRequestResult::Failed;
        if (bHaveReachable)
        {
            R = AI->MoveToLocation(Reachable, Acceptance, true, true, true, false);
        }
        else
        {
            // fallback to actor target if projection fails
            R = AI->MoveToActor(Actor, Acceptance, true, true, true, 0, true);
        }
        const bool bReqOK = (R != EPathFollowingRequestResult::Failed);
        UE_LOG(LogNyangCraft, Log, TEXT("[RTS] %s MoveTo %s → %s (via %s)"), *GetName(), *Actor->GetName(), bReqOK ? TEXT("OK") : TEXT("Failed"), bHaveReachable ? TEXT("Location") : TEXT("Actor"));
        if (!bReqOK)
        {
            FinishAllTasks();
            return false;
        }
        // Start polling for arrival
        MoveTargetActor = Actor;
        MoveAcceptance = Acceptance;
        bHasMoveTargetLocation = bHaveReachable;
        MoveTargetLocation = bHaveReachable ? Reachable : FVector::ZeroVector;
        auto Poll = [this]()
        {
            if (!MoveTargetActor.IsValid())
            {
                GetWorldTimerManager().ClearTimer(MovePollTimer);
                FinishAllTasks();
                return;
            }
            // Refresh debug visuals while moving
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
            if (UWorld* World = GetWorld())
            {
                float BoundsR = 0.f;
                if (const UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(MoveTargetActor->GetRootComponent()))
                {
                    BoundsR = Prim->Bounds.SphereRadius;
                }
                const bool bTiny = BoundsR < 10.f;
                const float EffectiveR = bTiny ? 100.f : BoundsR;
                const FColor Pink(255, 105, 180);
                const FColor BoundsColor = bTiny ? FColor::Red : Pink;
                DrawDebugSphere(World, MoveTargetActor->GetActorLocation(), EffectiveR, 24, BoundsColor, false, 0.15f, 0, 1.5f);
                // Worker-centered acceptance radius (green)
                DrawDebugSphere(World, GetActorLocation(), MoveAcceptance, 24, FColor::Green, false, 0.15f, 0, 0.75f);
            }
#endif
            const bool bNearActor = IsNearActor2D(MoveTargetActor.Get(), MoveAcceptance);
            const bool bNearLoc = bHasMoveTargetLocation ? (FVector::Dist2D(GetActorLocation(), MoveTargetLocation) <= FMath::Max(75.f, MoveAcceptance)) : false;
            if (bNearActor || bNearLoc)
            {
                GetWorldTimerManager().ClearTimer(MovePollTimer);
                if (Phase == EWorkerPhase::MoveToResource && TargetResource.IsValid())
                {
                    StartGatherTick();
                }
                else if (Phase == EWorkerPhase::MoveToDropoff && DropoffTarget.IsValid())
                {
                    StartDeposit();
                }
            }
        };
        // 10 Hz polling is fine here
        GetWorldTimerManager().SetTimer(MovePollTimer, FTimerDelegate::CreateLambda(Poll), 0.1f, true);
        return true;
    }
    return false;
}

void ARTSWorker::UpdateStatusText()
{
    if (!StatusText) return;
    FString StateStr;
    switch (Phase)
    {
    case EWorkerPhase::Idle: StateStr = TEXT("Idle"); break;
    case EWorkerPhase::MoveToResource: StateStr = TEXT("To Resource"); break;
    case EWorkerPhase::Gather: StateStr = TEXT("Gathering"); break;
    case EWorkerPhase::MoveToDropoff: StateStr = TEXT("To Dropoff"); break;
    case EWorkerPhase::Deposit: StateStr = TEXT("Depositing"); break;
    }
    const FString CarryStr = FString::Printf(TEXT(" %d/%d"), Carried, CarryCapacity);
    StatusText->SetText(FText::FromString(StateStr + CarryStr));
}

void ARTSWorker::FaceCamera()
{
    if (!StatusText) return;
    UWorld* World = GetWorld();
    if (!World) return;
    APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(World, 0);
    if (!PCM) return;
    const FVector ToCam = PCM->GetCameraLocation() - StatusText->GetComponentLocation();
    FRotator LookRot = FRotationMatrix::MakeFromX(ToCam).Rotator();
    if (bBillboardYawOnly)
    {
        LookRot.Pitch = 0.f;
        LookRot.Roll = 0.f;
    }
    StatusText->SetWorldRotation(LookRot);
}

bool ARTSWorker::IsNearActor2D(AActor* Actor, float Acceptance) const
{
    if (!Actor) return false;
    float Radius = 0.f;
    if (const UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
    {
        Radius = Prim->Bounds.SphereRadius;
    }
    if (Radius < 10.f)
    {
        Radius = 100.f; // Force minimum effective radius for tiny targets
    }
    const float DistSurface = FMath::Max(0.f, FVector::Dist2D(GetActorLocation(), Actor->GetActorLocation()) - Radius);
    return DistSurface <= Acceptance;
}
bool ARTSWorker::MoveToLocationOnNav(const FVector& GoalLocation, float Acceptance)
{
    if (AAIController* AI = Cast<AAIController>(GetController()))
    {
        const EPathFollowingRequestResult::Type R = AI->MoveToLocation(GoalLocation, Acceptance, true, true, true, false);
        return R != EPathFollowingRequestResult::Failed;
    }
    return false;
}

bool ARTSWorker::FindReachablePointNear(const FVector& Around, FVector& OutPoint, float SearchExtent) const
{
    UWorld* World = GetWorld();
    if (!World) return false;
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
    if (!NavSys) return false;
    const FVector Extent(SearchExtent, SearchExtent, SearchExtent);
    FNavLocation Projected;
    if (NavSys->ProjectPointToNavigation(Around, Projected, Extent))
    {
        OutPoint = Projected.Location;
        return true;
    }
    // Try a wider search
    if (NavSys->ProjectPointToNavigation(Around, Projected, Extent * 2.f))
    {
        OutPoint = Projected.Location;
        return true;
    }
    return false;
}
