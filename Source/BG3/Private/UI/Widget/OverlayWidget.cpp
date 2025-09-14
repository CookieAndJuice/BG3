
#include "UI/Widget/OverlayWidget.h"

#include "Game/BG3GameManageSubsystem.h"
#include "UI/Widget/CombatActionPanel.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widget/TurnEndWidget.h"

void UOverlayWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Init Turn UI
    UBG3GameManageSubsystem* GMSubsys = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>();
    TurnEndWidget->OnTurnEndDelegate.BindUFunction(GMSubsys, FName(TEXT("BeginNextTurn")));
}

void UOverlayWidget::SetController(UOverlayWidgetController* InController)
{
    Controller = InController;

    if (ActionPanel)
    {
        ActionPanel->SetController(Controller);
    }
}