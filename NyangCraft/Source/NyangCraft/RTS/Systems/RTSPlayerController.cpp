#include "RTS/Systems/RTSPlayerController.h"
#include "RTS/Units/RTSUnit.h"
#include "Engine/World.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ARTSPlayerController::ARTSPlayerController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;
}

void ARTSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    // Using polling in PlayerTick via WasInputKeyJustPressed to avoid EnhancedInput asset setup.
}

void ARTSPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if (WasInputKeyJustPressed(EKeys::LeftMouseButton))
    {
        HandleSelection();
    }

    if (WasInputKeyJustPressed(EKeys::RightMouseButton))
    {
        HandleCommand();
    }

    ClearInvalidSelections();
}

void ARTSPlayerController::HandleSelection()
{
    FHitResult Hit;
    if (TraceUnderCursor(Hit))
    {
        if (ARTSUnit* HitUnit = Cast<ARTSUnit>(Hit.GetActor()))
        {
            // Clear previous selection visual state
            for (TWeakObjectPtr<ARTSUnit>& UnitPtr : SelectedUnits)
            {
                if (ARTSUnit* U = UnitPtr.Get())
                {
                    U->SetSelected(false);
                }
            }
            // Single selection for now
            SelectedUnits.Reset();
            SelectedUnits.Add(HitUnit);
            HitUnit->SetSelected(true);
        }
    }
}

void ARTSPlayerController::HandleCommand()
{
    FHitResult Hit;
    if (!TraceUnderCursor(Hit)) return;

    const FVector Dest = Hit.ImpactPoint;

    for (int32 i = SelectedUnits.Num() - 1; i >= 0; --i)
    {
        if (ARTSUnit* Unit = SelectedUnits[i].Get())
        {
            Unit->IssueMoveOrder(Dest);
        }
    }
}

bool ARTSPlayerController::TraceUnderCursor(FHitResult& OutHit, ECollisionChannel Channel) const
{
    bool bHit = GetHitResultUnderCursor(Channel, true, OutHit);
    return bHit;
}

void ARTSPlayerController::ClearInvalidSelections()
{
    for (int32 i = SelectedUnits.Num() - 1; i >= 0; --i)
    {
        if (!SelectedUnits[i].IsValid())
        {
            SelectedUnits.RemoveAtSwap(i);
        }
    }
}
