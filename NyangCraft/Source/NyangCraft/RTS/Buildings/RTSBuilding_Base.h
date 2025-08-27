#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RTSBuilding_Base.generated.h"

UCLASS()
class ARTSBuilding_Base : public AActor
{
    GENERATED_BODY()

public:
    ARTSBuilding_Base();
    virtual void Tick(float DeltaSeconds) override;
    virtual void BeginPlay() override;
    int32 GetMineralCost() const { return MineralCost; }
    bool IsUnderConstruction() const { return bUnderConstruction; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Root")
    USceneComponent* Root;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Building")
    int32 MaxHP = 1000;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Building")
    int32 HP = 1000;

    // Construction
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Building")
    int32 MineralCost = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Building")
    float BuildTime = 10.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Building")
    float BuildProgress = 0.f; // 0..1

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Building")
    bool bUnderConstruction = true;

    virtual void OnConstructionCompleted();
};

