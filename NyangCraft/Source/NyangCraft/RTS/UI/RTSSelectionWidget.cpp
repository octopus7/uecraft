#include "RTS/UI/RTSSelectionWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Rendering/DrawElements.h"

int32 URTSSelectionWidget::NativePaint(const FPaintArgs& Args,
                                       const FGeometry& AllottedGeometry,
                                       const FSlateRect& MyCullingRect,
                                       FSlateWindowElementList& OutDrawElements,
                                       int32 LayerId,
                                       const FWidgetStyle& InWidgetStyle,
                                       bool bParentEnabled) const
{
    const int32 RetLayer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    if (!bSelecting)
    {
        return RetLayer;
    }

    const float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
    const float MinX = FMath::Min(Start.X, End.X) / Scale;
    const float MaxX = FMath::Max(Start.X, End.X) / Scale;
    const float MinY = FMath::Min(Start.Y, End.Y) / Scale;
    const float MaxY = FMath::Max(Start.Y, End.Y) / Scale;

    const FVector2D A(MinX, MinY);
    const FVector2D B(MaxX, MinY);
    const FVector2D C(MaxX, MaxY);
    const FVector2D D(MinX, MaxY);

    const FPaintGeometry PG = AllottedGeometry.ToPaintGeometry();
    const ESlateDrawEffect Effects = ESlateDrawEffect::None;
    const bool bAA = true;

    {
        TArray<FVector2D> P; P.Add(A); P.Add(B);
        FSlateDrawElement::MakeLines(OutDrawElements, RetLayer, PG, P, Effects, OutlineColor, bAA, OutlineThickness);
    }
    {
        TArray<FVector2D> P; P.Add(B); P.Add(C);
        FSlateDrawElement::MakeLines(OutDrawElements, RetLayer, PG, P, Effects, OutlineColor, bAA, OutlineThickness);
    }
    {
        TArray<FVector2D> P; P.Add(C); P.Add(D);
        FSlateDrawElement::MakeLines(OutDrawElements, RetLayer, PG, P, Effects, OutlineColor, bAA, OutlineThickness);
    }
    {
        TArray<FVector2D> P; P.Add(D); P.Add(A);
        FSlateDrawElement::MakeLines(OutDrawElements, RetLayer, PG, P, Effects, OutlineColor, bAA, OutlineThickness);
    }

    return RetLayer + 1;
}
