#include "RTS/Buildings/RTSBuilding_SupplyDepot.h"

ARTSBuilding_SupplyDepot::ARTSBuilding_SupplyDepot()
{
    MineralCost = 20;
    BuildTime = 8.f;
    MaxHP = 600;
}

#include "RTS/Systems/RTSGameMode.h"
#include "Engine/World.h"

void ARTSBuilding_SupplyDepot::OnConstructionCompleted()
{
    Super::OnConstructionCompleted();
    // Grant supply cap on completion
    if (UWorld* World = GetWorld())
    {
        if (ARTSGameMode* GM = World->GetAuthGameMode<ARTSGameMode>())
        {
            GM->AddSupplyCap(8);
        }
    }
}
