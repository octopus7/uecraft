#pragma once

#include "CoreMinimal.h"
#include "RTS/Buildings/RTSBuilding_Base.h"
#include "RTSBuilding_Barracks.generated.h"

class ARTSMarine;

UCLASS()
class ARTSBuilding_Barracks : public ARTSBuilding_Base
{
    GENERATED_BODY()

public:
    ARTSBuilding_Barracks();

    UFUNCTION(BlueprintCallable, Category="RTS|Production")
    void QueueTrainMarine(int32 Count = 1);

    UFUNCTION(BlueprintPure, Category="RTS|Production")
    int32 GetQueueCount() const { return MarineQueue; }

    UFUNCTION(BlueprintPure, Category="RTS|Production")
    bool IsTraining() const { return bTraining; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Production")
    float TrainTimeMarine = 4.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Production")
    int32 MarineQueue = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Production")
    bool bTraining = false;

    // Spawn class for Marines (set to BP_Marine in Barracks BP defaults)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RTS|Production")
    TSubclassOf<ARTSMarine> MarineClass;

private:
    FTimerHandle TrainTimerHandle;
    int32 MarineSupplyCost = 1;
    void StartNextTraining();
    void FinishTrainingAndSpawn();
};
