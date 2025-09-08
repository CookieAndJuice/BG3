
#include "UI/Widget/CombatActionPanel.h"
#include "UI/Widget/ActionSlotEntry.h"
#include "UI/WidgetController/CombatActionWidgetController.h"
#include "Components/PanelWidget.h"

void UCombatActionPanel::SetController(UCombatActionWidgetController* InController)
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

    for (const FActionSlotView& View : Slots)
    {
        UActionSlotEntry* Entry = CreateWidget<UActionSlotEntry>(this, ActionSlotEntryClass);
        if (!Entry) continue;
        Entry->Setup(View);
        Panel_Root->AddChild(Entry);
    }
}
