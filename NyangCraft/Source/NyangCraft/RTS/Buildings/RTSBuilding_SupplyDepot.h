#pragma once

#include "CoreMinimal.h"
#include "RTS/Buildings/RTSBuilding_Base.h"
#include "RTSBuilding_SupplyDepot.generated.h"

UCLASS()
class ARTSBuilding_SupplyDepot : public ARTSBuilding_Base
{
    GENERATED_BODY()

public:
    ARTSBuilding_SupplyDepot();

protected:
    virtual void OnConstructionCompleted() override;
};
