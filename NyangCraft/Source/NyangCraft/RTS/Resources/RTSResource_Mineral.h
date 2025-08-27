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

public:
    UFUNCTION(BlueprintCallable, Category="RTS|Resource")
    int32 Harvest(int32 Request)
    {
        const int32 Taken = FMath::Clamp(Request, 0, Amount);
        Amount -= Taken;
        return Taken;
    }

    UFUNCTION(BlueprintPure, Category="RTS|Resource")
    bool IsDepleted() const { return Amount <= 0; }
};
