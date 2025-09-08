
#include "UI/Widget/ActionSlotEntry.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"

void UActionSlotEntry::Setup(const FActionSlotView& InView)
{
    View = InView;
    if (Img_Icon)
    {
        Img_Icon->SetBrushFromTexture(View.Icon);
    }

    if (Overlay_Disalbed)
    {
        Overlay_Disalbed->SetVisibility(View.bUsable ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
    }
}

void UActionSlotEntry::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (Btn_Use)
    {
        Btn_Use->OnClicked.AddDynamic(this, &UActionSlotEntry::HandleClick);
    }
}

void UActionSlotEntry::HandleClick()
{
    OnClicked.Broadcast(View.SkillID);
}
