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

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Building")
    int32 MaxHP = 1000;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Building")
    int32 HP = 1000;
};

