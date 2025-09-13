
#include "UI/Widget/CombatActionPanel.h"

#include "BG3/BG3.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "UI/Widget/ActionSlotEntry.h"
#include "Components/PanelWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UCombatActionPanel::SetController(UOverlayWidgetController* InController)
{
    Controller = InController;
    if (Controller)
    {
        Controller->OnActionSlotsUpdated.AddDynamic(this, &UCombatActionPanel::RebuildSlots);
        Controller->RefreshSlots();
    }
}

void UCombatActionPanel::RebuildSlots(const TArray<FActionSlotView>& Slots)
{
    if (!Panel_Root || !ActionSlotEntryClass)
    {
        return;
    }

    Panel_Root->ClearChildren();

    const int32 Rows = FMath::Max(1, NumRow);
    for (int32 Index = 0; Index < Slots.Num(); ++Index)
    {
        const FActionSlotView& View = Slots[Index];
        UActionSlotEntry* Entry = CreateWidget<UActionSlotEntry>(this, ActionSlotEntryClass);
        if (!Entry) continue;
        Entry->Setup(View);
        Entry->OnClicked.AddDynamic(this, &UCombatActionPanel::OnSkillButtonClicked);

        const int32 Row = Index % Rows;
        const int32 Col = Index / Rows;
        if (UGridSlot* GridSlot = Panel_Root->AddChildToGrid(Entry, Row, Col))
        {
            GridSlot->SetHorizontalAlignment(HAlign_Left);                                                                               
            GridSlot->SetVerticalAlignment(VAlign_Top);  
        }
    }
}

void UCombatActionPanel::OnSkillButtonClicked(int32 SkillID)
{
    Controller->RequestUseSkill(SkillID);
}
