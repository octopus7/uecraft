#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RTSResource_Mineral.generated.h"

UCLASS()
class ARTSResource_Mineral : public AActor
{
    GENERATED_BODY()
public:
    ARTSResource_Mineral();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    int32 Amount;

    // Exclusive worker occupying this mineral (limit concurrent workers to 1)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    TWeakObjectPtr<class ARTSWorker> OccupiedBy;

public:
    UFUNCTION(BlueprintCallable, Category="RTS|Resource")
    int32 Harvest(int32 Request);

    UFUNCTION(BlueprintPure, Category="RTS|Resource")
    bool IsDepleted() const;

    // Occupancy API
    bool TryClaim(class ARTSWorker* Worker);
    void ReleaseClaim(class ARTSWorker* Worker);
    bool IsOccupied() const { return OccupiedBy.IsValid(); }
    class ARTSWorker* GetOccupant() const { return OccupiedBy.Get(); }
};
