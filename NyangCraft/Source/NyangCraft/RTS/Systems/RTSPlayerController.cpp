#include "RTS/Systems/RTSPlayerController.h"
#include "RTS/Units/RTSUnit.h"
#include "RTS/Units/RTSWorker.h"
#include "RTS/Resources/RTSResource_Mineral.h"
#include "RTS/UI/RTSSelectionWidget.h"
#include "RTS/UI/RTSHUDWidget.h"
#include "RTS/UI/RTSActionBarWidget.h"
#include "RTS/Buildings/RTSBuilding_Barracks.h"
#include "RTS/Buildings/RTSBuilding_SupplyDepot.h"
#include "RTS/Buildings/RTSBuilding_CommandCenter.h"
#include "RTS/Buildings/RTSBuilding_Base.h"
#include "RTS/Systems/RTSGameMode.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NyangCraft.h"

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

    // Toggle build mode (polling)
    if (WasInputKeyJustPressed(EKeys::B))
    {
        TogglePlaceBarracksMode();
    }
    if (WasInputKeyJustPressed(EKeys::D))
    {
        TogglePlaceSupplyDepotMode();
    }

    // Handle building placement on left click when in build mode
    if (bPlacingBarracks && WasInputKeyJustPressed(EKeys::LeftMouseButton))
    {
        if (PlaceBarracksAtCursor())
        {
            bPlacingBarracks = false;
        }
        return; // avoid starting selection this frame
    }
    if (bPlacingSupplyDepot && WasInputKeyJustPressed(EKeys::LeftMouseButton))
    {
        if (PlaceSupplyDepotAtCursor())
        {
            bPlacingSupplyDepot = false;
        }
        return; // avoid starting selection this frame
    }

    if (WasInputKeyJustPressed(EKeys::LeftMouseButton))
    {
        BeginSelection();
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, TEXT("[RTS] 드래그 선택 시작"));
        }
        UE_LOG(LogNyangCraft, Log, TEXT("[RTS] Drag selection started"));
    }

    if (IsInputKeyDown(EKeys::LeftMouseButton) && bIsSelecting)
    {
        UpdateSelectionDrag();
    }

    if (WasInputKeyJustReleased(EKeys::LeftMouseButton) && bIsSelecting)
    {
        EndSelection();
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, TEXT("[RTS] 드래그 선택 종료"));
        }
        UE_LOG(LogNyangCraft, Log, TEXT("[RTS] Drag selection ended"));
    }

    if (WasInputKeyJustPressed(EKeys::RightMouseButton))
    {
        HandleCommand();
    }

    // Train Marine on M
    if (WasInputKeyJustPressed(EKeys::M))
    {
        FHitResult Hit;
        const bool bHit = TraceUnderCursor(Hit);
        const FVector Around = bHit ? Hit.ImpactPoint : FVector::ZeroVector;
        UWorld* World = GetWorld();
        if (World)
        {
            class ARTSBuilding_Barracks* Best = nullptr;
            float BestD2 = TNumericLimits<float>::Max();
            for (TActorIterator<ARTSBuilding_Barracks> It(World); It; ++It)
            {
                ARTSBuilding_Barracks* B = *It;
                if (!B || B->IsUnderConstruction()) continue;
                const float D2 = FVector::DistSquared(B->GetActorLocation(), Around);
                if (!bHit || D2 < BestD2)
                {
                    Best = B; BestD2 = D2;
                }
            }
            if (Best)
            {
                Best->QueueTrainMarine(1);
            }
            else if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Silver, TEXT("[RTS] 사용 가능한 배럭이 없습니다"));
            }
        }
    }

    // Train Worker on W
    if (WasInputKeyJustPressed(EKeys::W))
    {
        FHitResult Hit;
        const bool bHit = TraceUnderCursor(Hit);
        const FVector Around = bHit ? Hit.ImpactPoint : FVector::ZeroVector;
        UWorld* World = GetWorld();
        if (World)
        {
            class ARTSBuilding_CommandCenter* Best = nullptr;
            float BestD2 = TNumericLimits<float>::Max();
            for (TActorIterator<ARTSBuilding_CommandCenter> It(World); It; ++It)
            {
                ARTSBuilding_CommandCenter* CC = *It;
                if (!CC || CC->IsUnderConstruction()) continue;
                const float D2 = FVector::DistSquared(CC->GetActorLocation(), Around);
                if (!bHit || D2 < BestD2)
                {
                    Best = CC; BestD2 = D2;
                }
            }
            if (Best)
            {
                Best->QueueTrainWorker(1);
            }
            else if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Silver, TEXT("[RTS] 사용 가능한 커맨드센터가 없습니다"));
            }
        }
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
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("[RTS] 선택: %s"), *HitUnit->GetName()));
            }
            UE_LOG(LogNyangCraft, Log, TEXT("[RTS] Selected unit: %s"), *HitUnit->GetName());
        }
        else
        {
            // Clicked on empty space; clear selection
            ClearSelection();
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Silver, TEXT("[RTS] 선택 해제"));
            }
            UE_LOG(LogNyangCraft, Log, TEXT("[RTS] Selection cleared"));
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
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("[RTS] 채집 명령: %s"), *Mineral->GetName()));
        }
        UE_LOG(LogNyangCraft, Log, TEXT("[RTS] Issued gather order to mineral: %s"), *Mineral->GetName());
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
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::White,
            FString::Printf(TEXT("[RTS] 이동 명령: X=%.0f Y=%.0f"), Dest.X, Dest.Y));
    }
    UE_LOG(LogNyangCraft, Log, TEXT("[RTS] Move order to: %s"), *Dest.ToString());
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

void ARTSPlayerController::SingleClickSelect()
{
    FHitResult Hit;
    if (!TraceUnderCursor(Hit)) { ClearSelection(); return; }

    if (ARTSBuilding_Base* HitB = Cast<ARTSBuilding_Base>(Hit.GetActor()))
    {
        // Select building; clear units
        for (TWeakObjectPtr<ARTSUnit>& UnitPtr : SelectedUnits)
        {
            if (ARTSUnit* U = UnitPtr.Get())
            {
                U->SetSelected(false);
            }
        }
        SelectedUnits.Reset();
        SelectedBuilding = HitB;
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Green,
                FString::Printf(TEXT("[RTS] 건물 선택: %s"), *HitB->GetName()));
        }
        return;
    }

    if (ARTSUnit* HitUnit = Cast<ARTSUnit>(Hit.GetActor()))
    {
        // Select single unit
        for (TWeakObjectPtr<ARTSUnit>& UnitPtr : SelectedUnits)
        {
            if (ARTSUnit* U = UnitPtr.Get())
            {
                U->SetSelected(false);
            }
        }
        SelectedUnits.Reset();
        SelectedUnits.Add(HitUnit);
        HitUnit->SetSelected(true);
        SelectedBuilding = nullptr;
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Green,
                FString::Printf(TEXT("[RTS] 선택: %s"), *HitUnit->GetName()));
        }
        return;
    }

    // Empty space
    ClearSelection();
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
    if (SelectionWidget)
    {
        SelectionWidget->SetSelectionRect(true, SelectStart, SelectEnd);
    }
}

void ARTSPlayerController::UpdateSelectionDrag()
{
    float X, Y;
    if (GetMousePosition(X, Y))
    {
        SelectEnd = FVector2D(X, Y);
    }
    if (SelectionWidget)
    {
        SelectionWidget->SetSelectionRect(true, SelectStart, SelectEnd);
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
        SingleClickSelect();
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

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(
            TEXT("[RTS] 드래그 선택: %d 유닛"), SelectedUnits.Num()));
    }
    UE_LOG(LogNyangCraft, Log, TEXT("[RTS] Drag-selected %d units"), SelectedUnits.Num());
    if (SelectionWidget)
    {
        SelectionWidget->SetSelectionRect(false, FVector2D::ZeroVector, FVector2D::ZeroVector);
    }
}

void ARTSPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (!SelectionWidget)
    {
        SelectionWidget = CreateWidget<URTSSelectionWidget>(this, URTSSelectionWidget::StaticClass());
        if (SelectionWidget)
        {
            SelectionWidget->AddToViewport(1000);
        }
    }

    if (!HUDWidget)
    {
        HUDWidget = CreateWidget<URTSHUDWidget>(this, URTSHUDWidget::StaticClass());
        if (HUDWidget)
        {
            HUDWidget->AddToViewport(900);
        }
    }

    // Action bar at bottom center
    if (URTSActionBarWidget* ActionBar = CreateWidget<URTSActionBarWidget>(this, URTSActionBarWidget::StaticClass()))
    {
        ActionBar->SetOwnerController(this);
        ActionBar->AddToViewport(950);
        ActionBar->SetAnchorsInViewport(FAnchors(0.5f, 1.f));
        ActionBar->SetAlignmentInViewport(FVector2D(0.5f, 1.f));
        ActionBar->SetPositionInViewport(FVector2D(0.f, -10.f));
    }
}

void ARTSPlayerController::TogglePlaceBarracksMode()
{
    bPlacingBarracks = !bPlacingBarracks;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, bPlacingBarracks ? FColor::Yellow : FColor::Silver,
            bPlacingBarracks ? TEXT("[RTS] 배럭 배치 모드: 좌클릭으로 배치") : TEXT("[RTS] 배치 모드 종료"));
    }
}

bool ARTSPlayerController::PlaceBarracksAtCursor()
{
    FHitResult Hit;
    if (!TraceUnderCursor(Hit)) return false;

    UWorld* World = GetWorld();
    if (!World) return false;

    // Check cost from CDO
    UClass* BuildClass = BarracksClass ? BarracksClass.Get() : ARTSBuilding_Barracks::StaticClass();
    int32 Cost = 50;
    if (const ARTSBuilding_Base* CDO = Cast<ARTSBuilding_Base>(BuildClass->GetDefaultObject()))
    {
        Cost = CDO->GetMineralCost();
    }

    if (ARTSGameMode* GM = World->GetAuthGameMode<ARTSGameMode>())
    {
        if (!GM->TrySpendMinerals(Cost))
        {
            return false;
        }
    }

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    const FVector Loc = Hit.ImpactPoint;
    const FRotator Rot = FRotator::ZeroRotator;
    World->SpawnActor( BuildClass, &Loc, &Rot, Params );
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green,
            FString::Printf(TEXT("[RTS] 배럭 건설 시작: X=%.0f Y=%.0f"), Loc.X, Loc.Y));
    }
    return true;
}

void ARTSPlayerController::TogglePlaceSupplyDepotMode()
{
    bPlacingSupplyDepot = !bPlacingSupplyDepot;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, bPlacingSupplyDepot ? FColor::Yellow : FColor::Silver,
            bPlacingSupplyDepot ? TEXT("[RTS] 보급창 배치 모드: 좌클릭으로 배치") : TEXT("[RTS] 배치 모드 종료"));
    }
}

bool ARTSPlayerController::PlaceSupplyDepotAtCursor()
{
    FHitResult Hit;
    if (!TraceUnderCursor(Hit)) return false;
    UWorld* World = GetWorld();
    if (!World) return false;

    UClass* DepotClass = SupplyDepotClass ? SupplyDepotClass.Get() : ARTSBuilding_SupplyDepot::StaticClass();
    int32 Cost = 20;
    if (const ARTSBuilding_Base* CDO = Cast<ARTSBuilding_Base>(DepotClass->GetDefaultObject()))
    {
        Cost = CDO->GetMineralCost();
    }
    if (ARTSGameMode* GM = World->GetAuthGameMode<ARTSGameMode>())
    {
        if (!GM->TrySpendMinerals(Cost))
        {
            return false;
        }
    }
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    const FVector Loc2 = Hit.ImpactPoint;
    const FRotator Rot2 = FRotator::ZeroRotator;
    World->SpawnActor( DepotClass, &Loc2, &Rot2, Params );
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green,
            FString::Printf(TEXT("[RTS] 보급창 건설 시작: X=%.0f Y=%.0f"), Hit.ImpactPoint.X, Hit.ImpactPoint.Y));
    }
    return true;
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
    SelectedBuilding = nullptr;
    UE_LOG(LogNyangCraft, Log, TEXT("[RTS] Selection cleared (Reset)"));
}

void ARTSPlayerController::UI_TrainWorker()
{
    UWorld* World = GetWorld();
    if (!World) return;
    if (ARTSBuilding_CommandCenter* CC = Cast<ARTSBuilding_CommandCenter>(SelectedBuilding.Get()))
    {
        CC->QueueTrainWorker(1);
        return;
    }
    // fallback: nearest completed CC
    float BestD2 = TNumericLimits<float>::Max();
    ARTSBuilding_CommandCenter* Best = nullptr;
    for (TActorIterator<ARTSBuilding_CommandCenter> It(World); It; ++It)
    {
        ARTSBuilding_CommandCenter* C = *It;
        if (!C || C->IsUnderConstruction()) continue;
        const float D2 = FVector::DistSquared(C->GetActorLocation(), GetPawn() ? GetPawn()->GetActorLocation() : FVector::ZeroVector);
        if (D2 < BestD2) { BestD2 = D2; Best = C; }
    }
    if (Best) Best->QueueTrainWorker(1);
}

void ARTSPlayerController::UI_TrainMarine()
{
    UWorld* World = GetWorld();
    if (!World) return;
    if (ARTSBuilding_Barracks* B = Cast<ARTSBuilding_Barracks>(SelectedBuilding.Get()))
    {
        B->QueueTrainMarine(1);
        return;
    }
    // fallback: nearest completed Barracks
    float BestD2 = TNumericLimits<float>::Max();
    ARTSBuilding_Barracks* Best = nullptr;
    for (TActorIterator<ARTSBuilding_Barracks> It(World); It; ++It)
    {
        ARTSBuilding_Barracks* Bb = *It;
        if (!Bb || Bb->IsUnderConstruction()) continue;
        const float D2 = FVector::DistSquared(Bb->GetActorLocation(), GetPawn() ? GetPawn()->GetActorLocation() : FVector::ZeroVector);
        if (D2 < BestD2) { BestD2 = D2; Best = Bb; }
    }
    if (Best) Best->QueueTrainMarine(1);
}
