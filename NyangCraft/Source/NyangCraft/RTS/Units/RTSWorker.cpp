#include "RTS/Units/RTSWorker.h"
#include "RTS/Resources/RTSResource_Mineral.h"
#include "RTS/Buildings/RTSBuilding_CommandCenter.h"
#include "RTS/Systems/RTSGameMode.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
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
                }
                else
                {
                    State = EWorkerState::Idle;
                }
            }
            else
            {
                // Continue gathering
                StateTimer = GatherDuration;
            }
        }
        break;

    case EWorkerState::MovingToDropoff:
        if (!DropoffTarget.IsValid()) { State = EWorkerState::Idle; break; }
        if (IsNearActor(DropoffTarget.Get(), AcceptanceRadius))
        {
            State = EWorkerState::Depositing;
            StateTimer = DepositDuration;
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
                Carried = 0;
            }

            // Resume gathering if resource remains
            if (TargetResource.IsValid() && !TargetResource->IsDepleted())
            {
                MoveToActor(TargetResource.Get(), AcceptanceRadius);
                State = EWorkerState::MovingToResource;
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
}

void ARTSWorker::MoveToActor(AActor* Actor, float Acceptance)
{
    if (!Actor) return;
    if (AAIController* AI = Cast<AAIController>(GetController()))
    {
        AI->MoveToActor(Actor, Acceptance, true, true, true, 0, true);
    }
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
