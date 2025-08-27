#pragma once

#include "CoreMinimal.h"
#include "RTS/Units/RTSUnit.h"
#include "TimerManager.h"

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
    // Resource carrying
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    int32 CarryCapacity = 5;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    int32 Carried = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    float GatherDuration = 2.0f; // seconds per harvest tick

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    int32 GatherPerTick = 5; // minerals per harvest

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    float DepositDuration = 0.5f; // seconds to deposit

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    float AcceptanceRadius = 250.f; // distance to interact

private:
    // Fresh, event-driven worker phases
    enum class EWorkerPhase : uint8
    {
        Idle,
        MoveToResource,
        Gather,
        MoveToDropoff,
        Deposit
    };

    EWorkerPhase Phase = EWorkerPhase::Idle;

    // Targets
    TWeakObjectPtr<class ARTSResource_Mineral> TargetResource;
    TWeakObjectPtr<class ARTSBuilding_CommandCenter> DropoffTarget;

    // Timers
    FTimerHandle GatherTimerHandle;
    FTimerHandle DepositTimerHandle;

    // Flow control
    void MoveToResource();
    void MoveToDropoff();
    void StartGatherTick();
    void DoGatherTick();
    void StartDeposit();
    void DoDeposit();
    void FinishAllTasks();

    // Helpers
    class ARTSBuilding_CommandCenter* FindNearestCommandCenter() const;
    bool MoveToActor(AActor* Actor, float Acceptance);
    bool MoveToLocationOnNav(const FVector& GoalLocation, float Acceptance);
    bool FindReachablePointNear(const FVector& Around, FVector& OutPoint, float SearchExtent = 400.f) const;
    void UpdateStatusText();

    // Overhead status
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|UI", meta=(AllowPrivateAccess="true"))
    class UTextRenderComponent* StatusText;

    void FaceCamera();
    UPROPERTY(EditAnywhere, Category="RTS|UI")
    bool bBillboardYawOnly = false; // face camera on all axes by default

    // Move polling (avoid engine callback type in UFUNCTION)
    TWeakObjectPtr<AActor> MoveTargetActor;
    float MoveAcceptance = 0.f;
    FTimerHandle MovePollTimer;
    bool IsNearActor2D(AActor* Actor, float Acceptance) const;
    FVector MoveTargetLocation = FVector::ZeroVector;
    bool bHasMoveTargetLocation = false;
};
