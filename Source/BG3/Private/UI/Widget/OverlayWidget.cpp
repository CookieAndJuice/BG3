
#include "UI/Widget/OverlayWidget.h"
#include "UI/Widget/CombatActionPanel.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UOverlayWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Init Turn UI
    // TurnWidget->OnTurnEndDelegate.BindUFunction()
}

void UOverlayWidget::SetController(UOverlayWidgetController* InController)
{
    Controller = InController;

    if (ActionPanel)
    {
        ActionPanel->SetController(Controller);
    }
}