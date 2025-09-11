
#include "UI/Widget/ActionSlotEntry.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Styling/SlateTypes.h"
// For MakeBrushFromTexture and texture size
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/Texture2D.h"

void UActionSlotEntry::Setup(const FActionSlotView& InView)
{
    View = InView;
    if (Img_Icon)
    {
        // Match the brush size to the texture so the widget has a non-zero desired size
        Img_Icon->SetBrushFromTexture(View.Icon, /*bMatchSize=*/true);
    }

    if (Btn_Use)
    {
        // Keep a reference to the BG texture for GC safety
        CachedIconBG = View.IconBG;

        // Build a fresh style and assign via SetStyle (WidgetStyle is deprecated)
        FButtonStyle NewStyle = Btn_Use->GetStyle();
        NewStyle.NormalPadding = FMargin(0.f);
        NewStyle.PressedPadding = FMargin(0.f);

        if (CachedIconBG)
        {
            const int32 W = CachedIconBG->GetSizeX();
            const int32 H = CachedIconBG->GetSizeY();
            const FSlateBrush Brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(CachedIconBG, W, H);
            NewStyle.Normal   = Brush;
            NewStyle.Hovered  = Brush;
            NewStyle.Pressed  = Brush;
            NewStyle.Disabled = Brush;
        }

        Btn_Use->SetStyle(NewStyle);
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
