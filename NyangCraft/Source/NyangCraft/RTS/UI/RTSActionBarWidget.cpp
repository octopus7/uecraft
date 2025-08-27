#include "RTS/UI/RTSActionBarWidget.h"
#include "RTS/Systems/RTSPlayerController.h"
#include "RTS/Buildings/RTSBuilding_Barracks.h"
#include "RTS/Buildings/RTSBuilding_CommandCenter.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Styling/CoreStyle.h"

void URTSActionBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // One-time on-screen message to confirm initialization
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("[RTS] ActionBar initialized"));
    }
}

TSharedRef<SWidget> URTSActionBarWidget::RebuildWidget()
{
    if (!WidgetTree)
    {
        WidgetTree = NewObject<UWidgetTree>(this, UWidgetTree::StaticClass());
    }
    if (WidgetTree)
    {
        ButtonBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
        WidgetTree->RootWidget = ButtonBox;

        // Ensure we have some initial content before the Slate widget is built
        ClearButtons();
        if (UTextBlock* Label = MakeText(TEXT("Action Bar")))
        {
            const FSlateFontInfo LabelFont = FCoreStyle::GetDefaultFontStyle("Bold", 14);
            Label->SetFont(LabelFont);
            Label->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.9f, 0.2f, 1.f)));
        }
        MakeText(TEXT("|"));
        // Add context-specific buttons/text
        RebuildIfNeeded();
    }
    return Super::RebuildWidget();
}

void URTSActionBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    RebuildIfNeeded();
}

void URTSActionBarWidget::RebuildIfNeeded()
{
    int32 Queue = -1;
    const EContext Ctx = ComputeContext(Queue);
    if (Ctx != CachedContext || Queue != CachedQueue)
    {
        CachedContext = Ctx;
        CachedQueue = Queue;
        ClearButtons();

        // Persistent label on the left so the bar is always visible
        if (UTextBlock* Label = MakeText(TEXT("Action Bar")))
        {
            const FSlateFontInfo LabelFont = FCoreStyle::GetDefaultFontStyle("Bold", 14);
            Label->SetFont(LabelFont);
            Label->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.9f, 0.2f, 1.f))); // gold-ish
        }

        // Optional separator for readability
        MakeText(TEXT("|"));

        // Add small debug label to visually confirm widget presence
        if (bShowDebugLabel)
        {
            if (UTextBlock* Dbg = MakeText(TEXT("ActionBar Init")))
            {
                // Make it slightly subtle
                Dbg->SetColorAndOpacity(FSlateColor(FLinearColor(0.9f, 0.9f, 0.2f, 0.9f)));
                const FSlateFontInfo Font = FCoreStyle::GetDefaultFontStyle("Regular", 12);
                Dbg->SetFont(Font);
            }
        }
        switch (CachedContext)
        {
        case EContext::Build:
            {
                if (UButton* B1 = MakeButton(TEXT("Build: Barracks"))) { B1->OnClicked.AddDynamic(this, &URTSActionBarWidget::OnClickBuildBarracks); }
                if (UButton* B2 = MakeButton(TEXT("Build: Depot"))) { B2->OnClicked.AddDynamic(this, &URTSActionBarWidget::OnClickBuildDepot); }
                break;
            }
        case EContext::Barracks:
            {
                if (UButton* B = MakeButton(TEXT("Train: Marine"))) { B->OnClicked.AddDynamic(this, &URTSActionBarWidget::OnClickTrainMarine); }
                UTextBlock* Q = MakeText(FString::Printf(TEXT("Queue: %d"), CachedQueue));
                break;
            }
        case EContext::CommandCenter:
            {
                if (UButton* B = MakeButton(TEXT("Train: Worker"))) { B->OnClicked.AddDynamic(this, &URTSActionBarWidget::OnClickTrainWorker); }
                UTextBlock* Q = MakeText(FString::Printf(TEXT("Queue: %d"), CachedQueue));
                break;
            }
        default:
            break;
        }
    }
}

URTSActionBarWidget::EContext URTSActionBarWidget::ComputeContext(int32& OutQueue) const
{
    OutQueue = -1;
    if (!OwnerPC) return EContext::None;
    if (OwnerPC->IsPlacingBarracksMode() || OwnerPC->IsPlacingSupplyDepotMode())
    {
        return EContext::Build;
    }
    if (ARTSBuilding_Base* Sel = OwnerPC->GetSelectedBuilding())
    {
        if (ARTSBuilding_Barracks* B = Cast<ARTSBuilding_Barracks>(Sel))
        {
            OutQueue = B->GetQueueCount();
            return EContext::Barracks;
        }
        if (ARTSBuilding_CommandCenter* C = Cast<ARTSBuilding_CommandCenter>(Sel))
        {
            OutQueue = C->GetQueueCount();
            return EContext::CommandCenter;
        }
    }
    return EContext::Build; // default to build options when no selection
}

void URTSActionBarWidget::ClearButtons()
{
    if (!ButtonBox) return;
    ButtonBox->ClearChildren();
}

UButton* URTSActionBarWidget::MakeButton(const FString& Label)
{
    if (!ButtonBox) return nullptr;
    UButton* Btn = WidgetTree ? WidgetTree->ConstructWidget<UButton>(UButton::StaticClass()) : NewObject<UButton>(this);
    UTextBlock* Txt = WidgetTree ? WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass()) : NewObject<UTextBlock>(this);
    Txt->SetText(FText::FromString(Label));
    Btn->AddChild(Txt);
    UHorizontalBoxSlot* AddedSlot = ButtonBox->AddChildToHorizontalBox(Btn);
    AddedSlot->SetPadding(ButtonPadding);
    return Btn;
}

UTextBlock* URTSActionBarWidget::MakeText(const FString& Label)
{
    if (!ButtonBox) return nullptr;
    UTextBlock* Txt = WidgetTree ? WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass()) : NewObject<UTextBlock>(this);
    Txt->SetText(FText::FromString(Label));
    UHorizontalBoxSlot* AddedSlot = ButtonBox->AddChildToHorizontalBox(Txt);
    AddedSlot->SetPadding(ButtonPadding);
    return Txt;
}

void URTSActionBarWidget::OnClickBuildBarracks()
{
    if (OwnerPC) OwnerPC->UI_BuildBarracks();
}

void URTSActionBarWidget::OnClickBuildDepot()
{
    if (OwnerPC) OwnerPC->UI_BuildSupplyDepot();
}

void URTSActionBarWidget::OnClickTrainWorker()
{
    if (OwnerPC) OwnerPC->UI_TrainWorker();
}

void URTSActionBarWidget::OnClickTrainMarine()
{
    if (OwnerPC) OwnerPC->UI_TrainMarine();
}
