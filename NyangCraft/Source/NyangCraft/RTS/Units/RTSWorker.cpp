#include "RTS/Units/RTSWorker.h"
#include "RTS/Resources/RTSResource_Mineral.h"
#include "RTS/Buildings/RTSBuilding_CommandCenter.h"
#include "RTS/Systems/RTSGameMode.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "Engine/Engine.h"
#include "NyangCraft.h"
#include <cfloat>

ARTSWorker::ARTSWorker()
{
    // Worker-specific defaults can be set here.
}

void ARTSWorker::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    switch (State)
    {
    case EWorkerState::Idle:
        break;

    case EWorkerState::MovingToResource:
        if (!TargetResource.IsValid()) { State = EWorkerState::Idle; break; }
        if (IsNearActor(TargetResource.Get(), AcceptanceRadius))
        {
            State = EWorkerState::Gathering;
            StateTimer = GatherDuration;
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Yellow,
                    FString::Printf(TEXT("[RTS] %s 채집 시작"), *GetName()));
            }
            UE_LOG(LogNyangCraft, Log, TEXT("[RTS] %s started gathering"), *GetName());
        }
        break;

    case EWorkerState::Gathering:
        if (!TargetResource.IsValid()) { State = EWorkerState::Idle; break; }
        StateTimer -= DeltaSeconds;
        if (StateTimer <= 0.f)
        {
            // Perform gather tick
            const int32 Space = FMath::Max(0, CarryCapacity - Carried);
            const int32 ToHarvest = FMath::Min(Space, GatherPerTick);
            if (ToHarvest <= 0) {
                // Already full
            }
            int32 Harvested = 0;
            if (ToHarvest > 0)
            {
                Harvested = TargetResource->Harvest(ToHarvest);
                Carried += Harvested;
            }
            // Decide next action
            const bool bFull = (Carried >= CarryCapacity);
            const bool bDepleted = !TargetResource.IsValid() || TargetResource->IsDepleted();

            if (bFull || bDepleted)
            {
                DropoffTarget = FindNearestCommandCenter();
                if (DropoffTarget.IsValid())
                {
                    MoveToActor(DropoffTarget.Get(), AcceptanceRadius);
                    State = EWorkerState::MovingToDropoff;
                    if (GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Cyan,
                            FString::Printf(TEXT("[RTS] %s 반납 이동 (적재:%d)"), *GetName(), Carried));
                    }
                    UE_LOG(LogNyangCraft, Log, TEXT("[RTS] %s moving to dropoff with %d"), *GetName(), Carried);
                }
                else
                {
                    State = EWorkerState::Idle;
                    if (GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
                            TEXT("[RTS] 반납 대상(커맨드센터) 없음: 채집 중단"));
                    }
                    UE_LOG(LogNyangCraft, Warning, TEXT("[RTS] No CommandCenter found for dropoff"));
                }
            }
            else
            {
                // Continue gathering
                StateTimer = GatherDuration;
            }
            if (Harvested > 0)
            {
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,
                        FString::Printf(TEXT("[RTS] %s 채집 +%d (적재:%d/%d)"), *GetName(), Harvested, Carried, CarryCapacity));
                }
                UE_LOG(LogNyangCraft, Log, TEXT("[RTS] %s harvested %d (carried %d/%d)"), *GetName(), Harvested, Carried, CarryCapacity);
            }
        }
        break;

    case EWorkerState::MovingToDropoff:
        if (!DropoffTarget.IsValid()) { State = EWorkerState::Idle; break; }
        if (IsNearActor(DropoffTarget.Get(), AcceptanceRadius))
        {
            State = EWorkerState::Depositing;
            StateTimer = DepositDuration;
            UE_LOG(LogNyangCraft, Log, TEXT("[RTS] %s depositing %d"), *GetName(), Carried);
        }
        break;

    case EWorkerState::Depositing:
        StateTimer -= DeltaSeconds;
        if (StateTimer <= 0.f)
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

            // Resume gathering if resource remains
            if (TargetResource.IsValid() && !TargetResource->IsDepleted())
            {
                const bool bReq = MoveToActor(TargetResource.Get(), AcceptanceRadius);
                State = EWorkerState::MovingToResource;
                if (bReq)
                {
                    UE_LOG(LogNyangCraft, Log, TEXT("[RTS] %s resuming gather"), *GetName());
                }
                else
                {
                    if (GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
                            TEXT("[RTS] 자원 근처 도달 불가(네비 없음)"));
                    }
                    UE_LOG(LogNyangCraft, Warning, TEXT("[RTS] %s cannot find reachable point near resource"), *GetName());
                }
            }
            else
            {
                State = EWorkerState::Idle;
            }
        }
        break;
    }
}

void ARTSWorker::IssueGatherOrder(ARTSResource_Mineral* InResource)
{
    if (!InResource) return;
    TargetResource = InResource;
    State = EWorkerState::MovingToResource;
    MoveToActor(InResource, AcceptanceRadius);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow,
            FString::Printf(TEXT("[RTS] %s 채집 명령 → %s"), *GetName(), *InResource->GetName()));
    }
    UE_LOG(LogNyangCraft, Log, TEXT("[RTS] %s issued gather order to %s"), *GetName(), *InResource->GetName());
}

bool ARTSWorker::MoveToActor(AActor* Actor, float Acceptance)
{
    if (!Actor) return false;
    return MoveToLocationOnNav(Actor->GetActorLocation(), Acceptance);
}

bool ARTSWorker::MoveToLocationOnNav(const FVector& GoalLocation, float Acceptance)
{
    FVector Reachable;
    if (!FindReachablePointNear(GoalLocation, Reachable, 400.f))
    {
        UE_LOG(LogNyangCraft, Warning, TEXT("[RTS] %s no reachable point near %s"), *GetName(), *GoalLocation.ToString());
        return false;
    }

    if (AAIController* AI = Cast<AAIController>(GetController()))
    {
        const EPathFollowingRequestResult::Type R = AI->MoveToLocation(Reachable, Acceptance, true, true, true, false);
        const TCHAR* RText = (R == EPathFollowingRequestResult::Failed)
            ? TEXT("Failed")
            : ((R == EPathFollowingRequestResult::AlreadyAtGoal) ? TEXT("AlreadyAtGoal") : TEXT("RequestSuccessful"));
        UE_LOG(LogNyangCraft, Log, TEXT("[RTS] %s MoveToLocation %s → %s"), *GetName(), *Reachable.ToString(), RText);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White,
                FString::Printf(TEXT("[RTS] 이동요청 %s (%s)"), *Reachable.ToCompactString(), RText));
        }
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
    // Expand search once
    if (NavSys->ProjectPointToNavigation(Around, Projected, Extent * 2.f))
    {
        OutPoint = Projected.Location;
        return true;
    }
    return false;
}

bool ARTSWorker::IsNearActor(AActor* Actor, float Acceptance) const
{
    if (!Actor) return false;
    return FVector::Dist2D(GetActorLocation(), Actor->GetActorLocation()) <= Acceptance;
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
