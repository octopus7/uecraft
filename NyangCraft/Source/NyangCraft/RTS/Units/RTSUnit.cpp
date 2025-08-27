#include "RTS/Units/RTSUnit.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ARTSUnit::ARTSUnit()
{
    bSelected = false;
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARTSUnit::SetSelected(bool bInSelected)
{
    bSelected = bInSelected;
    // Hook for visual feedback (e.g., selection decal) to be added later.
}

void ARTSUnit::IssueMoveOrder(const FVector& Destination)
{
    if (AAIController* AI = Cast<AAIController>(GetController()))
    {
        AI->MoveToLocation(Destination, /*AcceptanceRadius=*/5.f, /*bStopOnOverlap=*/true);
    }
    else
    {
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Destination);
    }
}

