#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.generated.h"

class ARTSUnit;
class ARTSResource_Mineral;
class URTSSelectionWidget;
class URTSHUDWidget;
class ARTSBuilding_Barracks;
class ARTSBuilding_SupplyDepot;

UCLASS()
class ARTSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ARTSPlayerController();
    virtual void PlayerTick(float DeltaTime) override;
    virtual void SetupInputComponent() override;
    virtual void BeginPlay() override;

protected:
    // selection
    UPROPERTY()
    TArray<TWeakObjectPtr<ARTSUnit>> SelectedUnits;

    UPROPERTY()
    TWeakObjectPtr<class ARTSBuilding_Base> SelectedBuilding;

    void HandleSelection();
    void SingleClickSelect();
    void HandleCommand();
    bool TraceUnderCursor(FHitResult& OutHit, ECollisionChannel Channel = ECC_Visibility) const;
    void ClearInvalidSelections();

    // Drag selection
    bool bIsSelecting = false;
    FVector2D SelectStart = FVector2D::ZeroVector;
    FVector2D SelectEnd = FVector2D::ZeroVector;
    float DragThreshold = 8.f; // pixels

    void BeginSelection();
    void UpdateSelectionDrag();
    void EndSelection();
    void ClearSelection();

    // UI
    UPROPERTY()
    URTSSelectionWidget* SelectionWidget = nullptr;

    UPROPERTY()
    URTSHUDWidget* HUDWidget = nullptr;

    // Build placement
    bool bPlacingBarracks = false;
    void TogglePlaceBarracksMode();
    bool PlaceBarracksAtCursor();

    bool bPlacingSupplyDepot = false;
    void TogglePlaceSupplyDepotMode();
    bool PlaceSupplyDepotAtCursor();

    // Configurable build classes (set to Blueprint classes in editor)
    UPROPERTY(EditDefaultsOnly, Category="RTS|Build")
    TSubclassOf<ARTSBuilding_Barracks> BarracksClass;

    UPROPERTY(EditDefaultsOnly, Category="RTS|Build")
    TSubclassOf<ARTSBuilding_SupplyDepot> SupplyDepotClass;

public:
    UFUNCTION(BlueprintPure, Category="RTS|UI")
    bool IsPlacingBarracksMode() const { return bPlacingBarracks; }

    UFUNCTION(BlueprintPure, Category="RTS|UI")
    bool IsPlacingSupplyDepotMode() const { return bPlacingSupplyDepot; }

    UFUNCTION(BlueprintPure, Category="RTS|UI")
    class ARTSBuilding_Base* GetSelectedBuilding() const { return SelectedBuilding.Get(); }

    UFUNCTION(BlueprintCallable, Category="RTS|UI")
    void UI_BuildBarracks() { bPlacingBarracks = true; bPlacingSupplyDepot = false; }

    UFUNCTION(BlueprintCallable, Category="RTS|UI")
    void UI_BuildSupplyDepot() { bPlacingSupplyDepot = true; bPlacingBarracks = false; }

    UFUNCTION(BlueprintCallable, Category="RTS|UI")
    void UI_TrainWorker();

    UFUNCTION(BlueprintCallable, Category="RTS|UI")
    void UI_TrainMarine();
};
