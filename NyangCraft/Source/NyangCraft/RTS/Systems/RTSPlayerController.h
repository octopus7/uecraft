#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.generated.h"

class ARTSUnit;
class ARTSResource_Mineral;
class URTSSelectionWidget;

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

    void HandleSelection();
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
};
