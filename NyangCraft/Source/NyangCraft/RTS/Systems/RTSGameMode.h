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

private:
    UPROPERTY(VisibleAnywhere, Category="RTS|Resource")
    int32 PlayerMinerals = 0;
};
