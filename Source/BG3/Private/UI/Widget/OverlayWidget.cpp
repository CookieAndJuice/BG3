
#include "UI/Widget/OverlayWidget.h"
#include "UI/Widget/CombatActionPanel.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UOverlayWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UOverlayWidget::SetController(UOverlayWidgetController* InController)
{
    Controller = InController;
}
