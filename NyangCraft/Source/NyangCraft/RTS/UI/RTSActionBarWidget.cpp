#include "RTS/UI/RTSActionBarWidget.h"
#include "RTS/Systems/RTSPlayerController.h"
#include "RTS/Buildings/RTSBuilding_Barracks.h"
#include "RTS/Buildings/RTSBuilding_CommandCenter.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

void URTSActionBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Create a simple horizontal button box as root content
    if (WidgetTree)
    {
        ButtonBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
        WidgetTree->RootWidget = ButtonBox;
    }
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
        switch (CachedContext)
        {
        case EContext::Build:
            {
                // Build buttons
                UButton* B1 = MakeButton(TEXT("Build: Barracks"), FSimpleDelegate::CreateUObject(this, &URTSActionBarWidget::OnClickBuildBarracks));
                UButton* B2 = MakeButton(TEXT("Build: Depot"), FSimpleDelegate::CreateUObject(this, &URTSActionBarWidget::OnClickBuildDepot));
                break;
            }
        case EContext::Barracks:
            {
                UButton* B = MakeButton(TEXT("Train: Marine"), FSimpleDelegate::CreateUObject(this, &URTSActionBarWidget::OnClickTrainMarine));
                UTextBlock* Q = MakeText(FString::Printf(TEXT("Queue: %d"), CachedQueue));
                break;
            }
        case EContext::CommandCenter:
            {
                UButton* B = MakeButton(TEXT("Train: Worker"), FSimpleDelegate::CreateUObject(this, &URTSActionBarWidget::OnClickTrainWorker));
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

UButton* URTSActionBarWidget::MakeButton(const FString& Label, FSimpleDelegate OnClick)
{
    if (!ButtonBox) return nullptr;
    UButton* Btn = WidgetTree ? WidgetTree->ConstructWidget<UButton>(UButton::StaticClass()) : NewObject<UButton>(this);
    UTextBlock* Txt = WidgetTree ? WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass()) : NewObject<UTextBlock>(this);
    Txt->SetText(FText::FromString(Label));
    Btn->AddChild(Txt);
    Btn->OnClicked.Add(OnClick);
    UHorizontalBoxSlot* Slot = ButtonBox->AddChildToHorizontalBox(Btn);
    Slot->SetPadding(ButtonPadding);
    return Btn;
}

UTextBlock* URTSActionBarWidget::MakeText(const FString& Label)
{
    if (!ButtonBox) return nullptr;
    UTextBlock* Txt = WidgetTree ? WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass()) : NewObject<UTextBlock>(this);
    Txt->SetText(FText::FromString(Label));
    UHorizontalBoxSlot* Slot = ButtonBox->AddChildToHorizontalBox(Txt);
    Slot->SetPadding(ButtonPadding);
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
