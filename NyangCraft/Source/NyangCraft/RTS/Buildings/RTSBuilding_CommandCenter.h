#pragma once

#include "CoreMinimal.h"
#include "RTS/Buildings/RTSBuilding_Base.h"
#include "RTSBuilding_CommandCenter.generated.h"

class ARTSWorker;

UCLASS()
class ARTSBuilding_CommandCenter : public ARTSBuilding_Base
{
    GENERATED_BODY()

public:
    ARTSBuilding_CommandCenter() = default;

    UFUNCTION(BlueprintCallable, Category="RTS|Production")
    void QueueTrainWorker(int32 Count = 1);

    UFUNCTION(BlueprintPure, Category="RTS|Production")
    int32 GetQueueCount() const { return WorkerQueue; }

    UFUNCTION(BlueprintPure, Category="RTS|Production")
    bool IsTraining() const { return bTraining; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Production")
    float TrainTimeWorker = 6.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Production")
    int32 WorkerQueue = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Production")
    bool bTraining = false;

    // Spawn class for Workers (set to BP_Worker in CommandCenter BP defaults)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RTS|Production")
    TSubclassOf<ARTSWorker> WorkerClass;

private:
    int32 WorkerSupplyCost = 1;
    FTimerHandle TrainTimerHandle;
    void StartNextTraining();
    void FinishTrainingAndSpawn();
};
