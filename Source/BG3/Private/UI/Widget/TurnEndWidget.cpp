// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/TurnEndWidget.h"

#include "Components/Button.h"
#include "Component/CharacterStatsComponent.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UTurnEndWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TurnEndButton)
	{
		TurnEndButton->OnClicked.AddDynamic(this, &UTurnEndWidget::OnTurnEndButtonClicked);
		TurnEndButton->OnPressed.AddDynamic(this, &UTurnEndWidget::OnTurnEndButtonPressed);
		TurnEndButton->OnReleased.AddDynamic(this, &UTurnEndWidget::OnTurnEndButtonReleased);
	}
}

void UTurnEndWidget::OnTurnEndButtonHovered()
{
	
}

void UTurnEndWidget::OnTurnEndButtonPressed()
{
	if (TurnEndButton)
	{
		TurnEndButton->SetCursor(EMouseCursor::Type::GrabHand);
	}
}

void UTurnEndWidget::OnTurnEndButtonReleased()
{
	if (TurnEndButton)
	{
		TurnEndButton->SetCursor(EMouseCursor::Type::Default);
	}
}

void UTurnEndWidget::OnTurnEndButtonClicked()
{
	OnTurnEndDelegate.ExecuteIfBound();
}



void UTurnEndWidget::BindController(UOverlayWidgetController* InController)
{
    if (Controller)
    {
        Controller->OnMoveDistanceChanged.RemoveDynamic(this, &UTurnEndWidget::HandleMoveDistanceChanged);
    }

    Controller = InController;

    if (!Controller)
    {
        return;
    }

    Controller->OnMoveDistanceChanged.AddDynamic(this, &UTurnEndWidget::HandleMoveDistanceChanged);

    if (Controller->Stats)
    {
        HandleMoveDistanceChanged(
            Controller->Stats->GetRemainingMoveDistance(),
            Controller->Stats->GetMaxMoveDistanceValue());
    }
}

void UTurnEndWidget::HandleMoveDistanceChanged(float Remaining, float Max)
{
    UpdateMoveDistance(Remaining, Max);
}

void UTurnEndWidget::NativeDestruct()
{
    if (Controller)
    {
        Controller->OnMoveDistanceChanged.RemoveDynamic(this, &UTurnEndWidget::HandleMoveDistanceChanged);
        Controller = nullptr;
    }

    Super::NativeDestruct();
}






