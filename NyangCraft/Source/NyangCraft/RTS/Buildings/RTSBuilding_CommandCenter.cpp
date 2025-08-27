#include "RTS/Buildings/RTSBuilding_CommandCenter.h"
#include "RTS/Units/RTSWorker.h"
#include "RTS/Systems/RTSGameMode.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

void ARTSBuilding_CommandCenter::QueueTrainWorker(int32 Count)
{
    if (IsUnderConstruction())
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Red, TEXT("[RTS] 건설 중: 생산 불가"));
        }
        return;
    }
    const int32 Add = FMath::Max(0, Count);
    WorkerQueue += Add;
    if (!bTraining && WorkerQueue > 0)
    {
        StartNextTraining();
    }
}

void ARTSBuilding_CommandCenter::StartNextTraining()
{
    if (WorkerQueue <= 0 || bTraining) return;
    // Check supply availability
    if (UWorld* World = GetWorld())
    {
        if (ARTSGameMode* GM = World->GetAuthGameMode<ARTSGameMode>())
        {
            if (!GM->TryConsumeSupply(WorkerSupplyCost))
            {
                // supply blocked; retry later
                GetWorldTimerManager().SetTimer(TrainTimerHandle, this, &ARTSBuilding_CommandCenter::StartNextTraining, 1.0f, false);
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("[RTS] 보급 부족: 워커 생산 대기"));
                }
                return;
            }
        }
    }

    bTraining = true;
    const float T = FMath::Max(0.f, TrainTimeWorker);
    if (T <= 0.f)
    {
        FinishTrainingAndSpawn();
        return;
    }
    GetWorldTimerManager().SetTimer(TrainTimerHandle, this, &ARTSBuilding_CommandCenter::FinishTrainingAndSpawn, T, false);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("[RTS] 워커 생산 시작"));
    }
}

void ARTSBuilding_CommandCenter::FinishTrainingAndSpawn()
{
    GetWorldTimerManager().ClearTimer(TrainTimerHandle);
    bTraining = false;

    UWorld* World = GetWorld();
    if (World)
    {
        const FVector BaseLoc = GetActorLocation();
        const FRotator Rot = GetActorRotation();
        const FVector Offset = GetActorForwardVector() * 250.f;
        const FVector SpawnLoc = BaseLoc + Offset;

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        UClass* SpawnClass = WorkerClass ? WorkerClass.Get() : ARTSWorker::StaticClass();
        World->SpawnActor(SpawnClass, &SpawnLoc, &Rot, Params);

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("[RTS] 워커 생산 완료"));
        }
    }

    WorkerQueue = FMath::Max(0, WorkerQueue - 1);
    if (WorkerQueue > 0)
    {
        StartNextTraining();
    }
}
