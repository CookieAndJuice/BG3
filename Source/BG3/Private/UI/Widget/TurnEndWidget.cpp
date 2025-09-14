// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/TurnEndWidget.h"

#include "Components/Button.h"

void UTurnEndWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TurnEndButton->OnClicked.AddDynamic(this, &UTurnEndWidget::OnTurnEndButtonClicked);
}

void UTurnEndWidget::OnTurnEndButtonClicked()
{
	OnTurnEndDelegate.ExecuteIfBound();
}
