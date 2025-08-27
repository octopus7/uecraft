#include "RTS/Units/RTSUnit.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Engine.h"
#include "NyangCraft.h"

ARTSUnit::ARTSUnit()
{
    bSelected = false;
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARTSUnit::SetSelected(bool bInSelected)
{
    bSelected = bInSelected;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f,
            bSelected ? FColor::Green : FColor::Silver,
            FString::Printf(TEXT("[RTS] %s %s"), *GetName(), bSelected ? TEXT("선택됨") : TEXT("선택해제")));
    }
    UE_LOG(LogNyangCraft, Log, TEXT("[RTS] %s %s"), *GetName(), bSelected ? TEXT("Selected") : TEXT("Deselected"));
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

