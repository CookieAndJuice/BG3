// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerHPWidget.h"

#include "Actor/BG3GameCamera.h"
#include "BG3/BG3.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Controller/BG3GameModePlayerController.h"
#include "Game/BG3GameManageSubsystem.h"
#include "Character/BaseCharacter.h"
#include "Character/BG3PlayerCharacter.h"

void UPlayerHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SlotMap.Add(1, {Player1HP, Player1HPBar});
	SlotMap.Add(2, {Player2HP, Player2HPBar});
	
	if (Player1Btn)
	{
		Player1Btn->OnClicked.AddDynamic(this, &UPlayerHPWidget::OnPlayer1BtnClicked);
	}
	if (Player2Btn)
	{
		Player2Btn->OnClicked.AddDynamic(this, &UPlayerHPWidget::OnPlayer2BtnClicked);
	}

	GMSubsys = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>();
}

void UPlayerHPWidget::SetPlayerHP(int32 id, int32 curHp, int32 maxHp)
{
	FString hpString = FString::Printf(TEXT("%d / %d"), curHp, maxHp);
	SlotMap[id].HpText->SetText(FText::FromString(hpString));

	float hp =  1 - (static_cast<float>(curHp) / maxHp);
	SlotMap[id].HpBar->SetPercent(hp);
}

void UPlayerHPWidget::OnPlayer1BtnClicked()
{
	if (ABG3GameModePlayerController* PC = Cast<ABG3GameModePlayerController>(GetOwningPlayer()))
	{
		if (PC->BG3Camera)
		{
			ABaseCharacter* character = Cast<ABaseCharacter>(GMSubsys->GetPlayerFromID(1));
			if (character)
			{
				PC->BG3Camera->FocusCamera(character);
			}
		}
	}
}

void UPlayerHPWidget::OnPlayer2BtnClicked()
{
	if (ABG3GameModePlayerController* PC = Cast<ABG3GameModePlayerController>(GetOwningPlayer()))
	{
		if (PC->BG3Camera)
		{
			ABaseCharacter* character = Cast<ABaseCharacter>(GMSubsys->GetPlayerFromID(2));
			if (character)
			{
				PC->BG3Camera->FocusCamera(character);
			}
		}
	}
}
