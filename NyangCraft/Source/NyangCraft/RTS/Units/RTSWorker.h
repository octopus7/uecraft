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

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    int32 CarryCapacity = 5;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Resource")
    int32 Carried = 0;
};

