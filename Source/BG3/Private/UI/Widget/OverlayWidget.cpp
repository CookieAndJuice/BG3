
#include "UI/Widget/OverlayWidget.h"

#include "BG3/BG3.h"
#include "Game/BG3GameManageSubsystem.h"
#include "Game/BG3GameMode.h"
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
        Controller->OnFadeOutAnimationStart.AddDynamic(this, &UOverlayWidget::FadeOutAnimationStart);
    }
}

void UOverlayWidget::FadeOutAnimationStart(EResultState result)
{
    PlayAnimation(FadeOut);

    PRINTDELEGATELOG(TEXT("Delegate 3"));

    if (result == EResultState::Enemy)
    {
        // Lose 띄우기
    }
    else if (result == EResultState::Player)
    {
        // Win 띄우기
    }
}
