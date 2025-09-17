// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerHPWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHPWidget::InitUI(int32 hp1, int32 hp2)
{
	FString hpString = FString::Printf(TEXT("%d / %d"), hp1, hp1);
	Player1HP->SetText(FText::FromString(hpString));
	hpString = FString::Printf(TEXT("%d / %d"), hp2, hp2);
	Player2HP->SetText(FText::FromString(hpString));

	maxHP1 = hp1;
	maxHP2 = hp2;
	
	Player1HPBar->SetPercent(0.f);
	Player2HPBar->SetPercent(0.f);
}

void UPlayerHPWidget::SetPlayer1HP(int32 hp1)
{
	FString hpString = FString::Printf(TEXT("%d / %d"), hp1, maxHP1);
	Player1HP->SetText(FText::FromString(hpString));

	float hp =  1 - (static_cast<float>(hp1) / maxHP1);
	Player1HPBar->SetPercent(hp);
}

void UPlayerHPWidget::SetPlayer2HP(int32 hp2)
{
	FString hpString = FString::Printf(TEXT("%d / %d"), hp2, maxHP2);
	Player1HP->SetText(FText::FromString(hpString));

	float hp = 1 - (static_cast<float>(hp2) / maxHP2);
	Player1HPBar->SetPercent(hp);
}
