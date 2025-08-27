#include "RTS/Systems/RTSPlayerController.h"
#include "RTS/Units/RTSUnit.h"
#include "RTS/Units/RTSWorker.h"
#include "RTS/Resources/RTSResource_Mineral.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
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
        BeginSelection();
    }

    if (IsInputKeyDown(EKeys::LeftMouseButton) && bIsSelecting)
    {
        UpdateSelectionDrag();
    }

    if (WasInputKeyJustReleased(EKeys::LeftMouseButton) && bIsSelecting)
    {
        EndSelection();
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
        else
        {
            // Clicked on empty space; clear selection
            ClearSelection();
        }
    }
}

void ARTSPlayerController::HandleCommand()
{
    FHitResult Hit;
    if (!TraceUnderCursor(Hit)) return;

    // If clicked on a mineral node and we have workers selected, issue gather
    if (ARTSResource_Mineral* Mineral = Cast<ARTSResource_Mineral>(Hit.GetActor()))
    {
        for (int32 i = SelectedUnits.Num() - 1; i >= 0; --i)
        {
            if (ARTSWorker* Worker = Cast<ARTSWorker>(SelectedUnits[i].Get()))
            {
                Worker->IssueGatherOrder(Mineral);
            }
        }
        return;
    }

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

void ARTSPlayerController::BeginSelection()
{
    bIsSelecting = true;
    float X, Y;
    if (GetMousePosition(X, Y))
    {
        SelectStart = FVector2D(X, Y);
        SelectEnd = SelectStart;
    }
}

void ARTSPlayerController::UpdateSelectionDrag()
{
    float X, Y;
    if (GetMousePosition(X, Y))
    {
        SelectEnd = FVector2D(X, Y);
    }
}

void ARTSPlayerController::EndSelection()
{
    bIsSelecting = false;

    const float Width = FMath::Abs(SelectEnd.X - SelectStart.X);
    const float Height = FMath::Abs(SelectEnd.Y - SelectStart.Y);

    if (Width < DragThreshold && Height < DragThreshold)
    {
        // Treat as single click selection
        HandleSelection();
        return;
    }

    // Build selection rect
    const float MinX = FMath::Min(SelectStart.X, SelectEnd.X);
    const float MaxX = FMath::Max(SelectStart.X, SelectEnd.X);
    const float MinY = FMath::Min(SelectStart.Y, SelectEnd.Y);
    const float MaxY = FMath::Max(SelectStart.Y, SelectEnd.Y);

    // Clear previous selection
    ClearSelection();

    // Iterate all units and select those inside rect
    for (TActorIterator<ARTSUnit> It(GetWorld()); It; ++It)
    {
        ARTSUnit* Unit = *It;
        if (!Unit) continue;

        FVector2D ScreenPos;
        if (ProjectWorldLocationToScreen(Unit->GetActorLocation(), ScreenPos, true))
        {
            if (ScreenPos.X >= MinX && ScreenPos.X <= MaxX && ScreenPos.Y >= MinY && ScreenPos.Y <= MaxY)
            {
                SelectedUnits.Add(Unit);
                Unit->SetSelected(true);
            }
        }
    }
}

void ARTSPlayerController::ClearSelection()
{
    for (TWeakObjectPtr<ARTSUnit>& UnitPtr : SelectedUnits)
    {
        if (ARTSUnit* U = UnitPtr.Get())
        {
            U->SetSelected(false);
        }
    }
    SelectedUnits.Reset();
}
