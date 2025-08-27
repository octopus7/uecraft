#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.generated.h"

class ARTSUnit;

UCLASS()
class ARTSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ARTSPlayerController();
    virtual void PlayerTick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

protected:
    // selection
    UPROPERTY()
    TArray<TWeakObjectPtr<ARTSUnit>> SelectedUnits;

    void HandleSelection();
    void HandleCommand();
    bool TraceUnderCursor(FHitResult& OutHit, ECollisionChannel Channel = ECC_Visibility) const;
    void ClearInvalidSelections();
};

