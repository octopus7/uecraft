#include "RTS/UI/RTSHUDWidget.h"
#include "RTS/Systems/RTSGameMode.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"

void URTSHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (UWorld* World = GetWorld())
    {
        if (ARTSGameMode* GM = World->GetAuthGameMode<ARTSGameMode>())
        {
            CachedMinerals = GM->GetMinerals();
        }
    }
}

int32 URTSHUDWidget::NativePaint(const FPaintArgs& Args,
                                 const FGeometry& AllottedGeometry,
                                 const FSlateRect& MyCullingRect,
                                 FSlateWindowElementList& OutDrawElements,
                                 int32 LayerId,
                                 const FWidgetStyle& InWidgetStyle,
                                 bool bParentEnabled) const
{
    const int32 RetLayer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    const float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
    const FVector2D ViewSize = AllottedGeometry.GetLocalSize();

    const FString Text = FString::Printf(TEXT("Minerals: %d"), CachedMinerals);
    const FSlateFontInfo Font = FCoreStyle::GetDefaultFontStyle("Regular", FontSize);

    // Approximate text size and compute panel size
    const float ApproxCharW = FontSize * 0.56f; // rough average glyph width
    const float ApproxW = Text.Len() * ApproxCharW;
    const float ApproxH = FontSize * 1.35f;
    const FVector2D PanelSize = FVector2D(ApproxW, ApproxH) + (PanelPadding * 2.f) / Scale;

    // Anchor to top-right: PanelOffset is from the right edge
    const FVector2D Origin = FVector2D(ViewSize.X - PanelSize.X - (PanelOffset.X / Scale), PanelOffset.Y / Scale);
    const FVector2D TextPos = Origin + PanelPadding / Scale;

    const FPaintGeometry PGPanel = AllottedGeometry.ToPaintGeometry(PanelSize, FSlateLayoutTransform(Origin));
    FSlateDrawElement::MakeBox(OutDrawElements, RetLayer, PGPanel, FCoreStyle::Get().GetBrush("WhiteBrush"), ESlateDrawEffect::None, PanelColor);

    const FPaintGeometry PGText = AllottedGeometry.ToPaintGeometry(FSlateLayoutTransform(TextPos));
    FSlateDrawElement::MakeText(OutDrawElements, RetLayer + 1, PGText, Text, Font, ESlateDrawEffect::None, TextColor);

    return RetLayer + 2;
}
