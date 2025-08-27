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
};

