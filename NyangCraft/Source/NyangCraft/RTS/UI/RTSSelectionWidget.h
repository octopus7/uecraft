#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTSSelectionWidget.generated.h"

UCLASS()
class URTSSelectionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetSelectionRect(bool bInSelecting, const FVector2D& InStart, const FVector2D& InEnd)
    {
        bSelecting = bInSelecting;
        Start = InStart;
        End = InEnd;
        InvalidateLayoutAndVolatility();
    }

protected:
    virtual int32 NativePaint(const FPaintArgs& Args,
                              const FGeometry& AllottedGeometry,
                              const FSlateRect& MyCullingRect,
                              FSlateWindowElementList& OutDrawElements,
                              int32 LayerId,
                              const FWidgetStyle& InWidgetStyle,
                              bool bParentEnabled) const override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|Selection")
    FLinearColor OutlineColor = FLinearColor(0.f, 0.9f, 0.3f, 1.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|Selection")
    float OutlineThickness = 2.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Selection")
    bool bSelecting = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Selection")
    FVector2D Start = FVector2D::ZeroVector;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RTS|Selection")
    FVector2D End = FVector2D::ZeroVector;
};
