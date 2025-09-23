// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/CharacterTurnHpWidget.h"


#include "Actor/BG3GameCamera.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Controller/BG3GameModePlayerController.h"
#include "Game/BG3GameManageSubsystem.h"
#include "Character/BaseCharacter.h"
#include "Character/BG3EnemyCharacter.h"
#include "Components/Image.h"


class ABG3GameModePlayerController;

void UCharacterTurnHpWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (CharacterBtn)
	{
		CharacterBtn->OnClicked.AddDynamic(this, &UCharacterTurnHpWidget::CharacterBtnClicked);
	}

	GMSubsys = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>();
}

void UCharacterTurnHpWidget::InitializeWithCharacter(ABaseCharacter* character)
{
	OwnerCharacter = character;
	if (Cast<ABG3EnemyCharacter>(OwnerCharacter.Get()))
	{
		CharacterBtnBG->SetColorAndOpacity({0.536458f, 0.036323f, 0.036323f, 1.f});
	}
	else
	{
		CharacterBtnBG->SetColorAndOpacity({0.033203f, 0.397256f, 0.708333f, 1.f});
	}
}

void UCharacterTurnHpWidget::SetPlayerHP(int32 curHp, int32 maxHp)
{
	float hp =  1 - (static_cast<float>(curHp) / maxHp);
	CharacterHPBar->SetPercent(hp);
}

void UCharacterTurnHpWidget::CharacterBtnClicked()
{
	if (ABG3GameModePlayerController* PC = Cast<ABG3GameModePlayerController>(GetOwningPlayer()))
	{
		if (PC->BG3Camera)
		{
			if (OwnerCharacter.IsValid())
			{
				PC->BG3Camera->FocusCamera(OwnerCharacter.Get());
			}
		}
	}
}	
