#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTSActionBarWidget.generated.h"

class UHorizontalBox;
class UButton;
class UTextBlock;
class ARTSPlayerController;
class ARTSBuilding_Barracks;
class ARTSBuilding_CommandCenter;

UCLASS()
class URTSActionBarWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    void SetOwnerController(ARTSPlayerController* InPC) { OwnerPC = InPC; }

protected:
    UPROPERTY()
    ARTSPlayerController* OwnerPC = nullptr;

    UPROPERTY()
    UHorizontalBox* ButtonBox = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|ActionBar")
    FMargin ButtonPadding = FMargin(6.f, 4.f);

private:
    enum class EContext : uint8 { None, Build, Barracks, CommandCenter };
    EContext CachedContext = EContext::None;
    int32 CachedQueue = -1;

    void RebuildIfNeeded();
    EContext ComputeContext(int32& OutQueue) const;
    void ClearButtons();
    UButton* MakeButton(const FString& Label, FSimpleDelegate OnClick);
    UTextBlock* MakeText(const FString& Label);

    // Handlers
    void OnClickBuildBarracks();
    void OnClickBuildDepot();
    void OnClickTrainWorker();
    void OnClickTrainMarine();
};

