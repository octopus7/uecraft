#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTSHUDWidget.generated.h"

UCLASS()
class URTSHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
    virtual int32 NativePaint(const FPaintArgs& Args,
                              const FGeometry& AllottedGeometry,
                              const FSlateRect& MyCullingRect,
                              FSlateWindowElementList& OutDrawElements,
                              int32 LayerId,
                              const FWidgetStyle& InWidgetStyle,
                              bool bParentEnabled) const override;

    // Style
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|HUD")
    FLinearColor PanelColor = FLinearColor(0.f, 0.f, 0.f, 0.55f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|HUD")
    FLinearColor TextColor = FLinearColor(0.1f, 0.9f, 0.9f, 1.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|HUD")
    int32 FontSize = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|HUD")
    FVector2D PanelPadding = FVector2D(12.f, 8.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|HUD")
    FVector2D PanelOffset = FVector2D(16.f, 16.f);

private:
    mutable int32 CachedMinerals = 0;
};

