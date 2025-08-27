#include "RTS/Buildings/RTSBuilding_Barracks.h"
#include "RTS/Units/RTSMarine.h"
#include "RTS/Systems/RTSGameMode.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "NyangCraft.h"

ARTSBuilding_Barracks::ARTSBuilding_Barracks()
{
    MineralCost = 50;
    BuildTime = 12.f;
    MaxHP = 1000;
}

void ARTSBuilding_Barracks::QueueTrainMarine(int32 Count)
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
    MarineQueue += Add;
    if (!bTraining && MarineQueue > 0)
    {
        StartNextTraining();
    }
}

void ARTSBuilding_Barracks::StartNextTraining()
{
    if (MarineQueue <= 0 || bTraining) return;
    // Check supply availability
    if (UWorld* World = GetWorld())
    {
        if (ARTSGameMode* GM = World->GetAuthGameMode<ARTSGameMode>())
        {
            if (!GM->TryConsumeSupply(MarineSupplyCost))
            {
                // supply blocked; retry later
                GetWorldTimerManager().SetTimer(TrainTimerHandle, this, &ARTSBuilding_Barracks::StartNextTraining, 1.0f, false);
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("[RTS] 보급 부족: 마린 생산 대기"));
                }
                return;
            }
        }
    }

    bTraining = true;
    const float T = FMath::Max(0.f, TrainTimeMarine);
    if (T <= 0.f)
    {
        FinishTrainingAndSpawn();
        return;
    }
    GetWorldTimerManager().SetTimer(TrainTimerHandle, this, &ARTSBuilding_Barracks::FinishTrainingAndSpawn, T, false);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("[RTS] 마린 훈련 시작"));
    }
}

void ARTSBuilding_Barracks::FinishTrainingAndSpawn()
{
    GetWorldTimerManager().ClearTimer(TrainTimerHandle);
    bTraining = false;

    UWorld* World = GetWorld();
    if (World)
    {
        const FVector BaseLoc = GetActorLocation();
        const FRotator Rot = GetActorRotation();
        // Spawn offset to the right of the Barracks
        const FVector Offset = GetActorRightVector() * 200.f;
        const FVector SpawnLoc = BaseLoc + Offset;

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        UClass* SpawnClass = MarineClass ? MarineClass.Get() : ARTSMarine::StaticClass();
        World->SpawnActor(SpawnClass, &SpawnLoc, &Rot, Params);

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("[RTS] 마린 훈련 완료"));
        }
    }

    MarineQueue = FMath::Max(0, MarineQueue - 1);
    if (MarineQueue > 0)
    {
        StartNextTraining();
    }
}
