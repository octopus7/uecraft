// RTS GameMode for NyangCraft

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RTSGameMode.generated.h"

UCLASS()
class ARTSGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ARTSGameMode();

    UFUNCTION(BlueprintCallable, Category="RTS|Resource")
    void AddMinerals(int32 Amount);

    UFUNCTION(BlueprintCallable, Category="RTS|Resource")
    int32 GetMinerals() const { return PlayerMinerals; }

    UFUNCTION(BlueprintCallable, Category="RTS|Resource")
    bool TrySpendMinerals(int32 Cost);

    // Supply API
    UFUNCTION(BlueprintCallable, Category="RTS|Supply")
    void AddSupplyCap(int32 Amount);

    UFUNCTION(BlueprintCallable, Category="RTS|Supply")
    bool TryConsumeSupply(int32 Amount);

    UFUNCTION(BlueprintCallable, Category="RTS|Supply")
    void ReleaseSupply(int32 Amount);

    UFUNCTION(BlueprintPure, Category="RTS|Supply")
    int32 GetSupplyUsed() const { return PlayerSupplyUsed; }

    UFUNCTION(BlueprintPure, Category="RTS|Supply")
    int32 GetSupplyCap() const { return PlayerSupplyCap; }

private:
    UPROPERTY(VisibleAnywhere, Category="RTS|Resource")
    int32 PlayerMinerals = 0;

    // Supply tracking
    UPROPERTY(VisibleAnywhere, Category="RTS|Supply")
    int32 PlayerSupplyUsed = 0;

    UPROPERTY(VisibleAnywhere, Category="RTS|Supply")
    int32 PlayerSupplyCap = 10; // base supply cap
};
