
#include "UI/Widget/CombatActionPanel.h"
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

    const int32 Columns = FMath::Max(1, NumColumns);
    for (int32 Index = 0; Index < Slots.Num(); ++Index)
    {
        const FActionSlotView& View = Slots[Index];
        UActionSlotEntry* Entry = CreateWidget<UActionSlotEntry>(this, ActionSlotEntryClass);
        if (!Entry) continue;
        Entry->Setup(View);
        Entry->OnClicked.AddDynamic(this, &UCombatActionPanel::OnSkillButtonClicked);

        const int32 Row = Index / Columns;
        const int32 Col = Index % Columns;
        if (UUniformGridSlot* GridSlot = Panel_Root->AddChildToUniformGrid(Entry, Row, Col))
        {
            GridSlot->SetHorizontalAlignment(HAlign_Fill);
            GridSlot->SetVerticalAlignment(VAlign_Fill);
        }
    }
}

void UCombatActionPanel::OnSkillButtonClicked(int32 SkillID)
{
    Controller->RequestUseSkill(SkillID);
}
