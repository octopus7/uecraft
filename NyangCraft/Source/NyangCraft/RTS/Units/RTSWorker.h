#pragma once

#include "CoreMinimal.h"
#include "RTS/Units/RTSUnit.h"
#include "RTSWorker.generated.h"

UCLASS()
class ARTSWorker : public ARTSUnit
{
    GENERATED_BODY()

public:
    ARTSWorker();
    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable)
    void IssueGatherOrder(class ARTSResource_Mineral* InResource);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    int32 CarryCapacity = 5;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    int32 Carried = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    float GatherDuration = 2.0f; // seconds per tick

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    int32 GatherPerTick = 5; // minerals per gather

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    float DepositDuration = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    float AcceptanceRadius = 120.f;

private:
    enum class EWorkerState : uint8
    {
        Idle,
        MovingToResource,
        Gathering,
        MovingToDropoff,
        Depositing
    };

    EWorkerState State = EWorkerState::Idle;
    TWeakObjectPtr<class ARTSResource_Mineral> TargetResource;
    TWeakObjectPtr<class ARTSBuilding_CommandCenter> DropoffTarget;
    float StateTimer = 0.f;

    void MoveToActor(AActor* Actor, float Acceptance = 100.f);
    bool IsNearActor(AActor* Actor, float Acceptance) const;
    class ARTSBuilding_CommandCenter* FindNearestCommandCenter() const;
};
