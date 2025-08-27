#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RTSUnit.generated.h"

UCLASS()
class ARTSUnit : public ACharacter
{
    GENERATED_BODY()

public:
    ARTSUnit();

    UFUNCTION(BlueprintCallable)
    void SetSelected(bool bInSelected);

    UFUNCTION(BlueprintCallable)
    void IssueMoveOrder(const FVector& Destination);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bSelected;
};

