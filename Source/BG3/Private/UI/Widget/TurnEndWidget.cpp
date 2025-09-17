// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/TurnEndWidget.h"

#include "Components/Button.h"

void UTurnEndWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	TurnEndButton->OnClicked.AddDynamic(this, &UTurnEndWidget::OnTurnEndButtonClicked);
	TurnEndButton->OnPressed.AddDynamic(this, &UTurnEndWidget::OnTurnEndButtonPressed);
	TurnEndButton->OnReleased.AddDynamic(this, &UTurnEndWidget::OnTurnEndButtonReleased);
}

void UTurnEndWidget::OnTurnEndButtonHovered()
{
	
}

void UTurnEndWidget::OnTurnEndButtonPressed()
{
	TurnEndButton->SetCursor(EMouseCursor::Type::GrabHand);
}

void UTurnEndWidget::OnTurnEndButtonReleased()
{
	TurnEndButton->SetCursor(EMouseCursor::Type::Default);
}

void UTurnEndWidget::OnTurnEndButtonClicked()
{
	OnTurnEndDelegate.ExecuteIfBound();
}
