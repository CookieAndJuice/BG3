// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/HpWidgetManageComponent.h"

#include "Character/BG3PlayerCharacter.h"
#include "Component/CharacterStatsComponent.h"
#include "Controller/BG3GameModePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/OverlayWidget.h"
#include "UI/Widget/PlayerHPWidget.h"

class UPlayerHPWidget;
// Sets default values for this component's properties
UHpWidgetManageComponent::UHpWidgetManageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UHpWidgetManageComponent::BeginPlay()
{
	Super::BeginPlay();

	if (auto* PC = Cast<ABG3GameModePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (UOverlayWidget* Overlay = PC->OverlayWidget)
		{
			PlayerHP = Overlay->GetPlayerHPWidget();
		}
	}

	if (UCharacterStatsComponent* playerStat = Cast<ABG3PlayerCharacter>(GetOwner())->Stats)
	{
		HandleHealthChanged(playerStat->GetHealth(), playerStat->GetMaxHealth());
	
		// Stat의 OnHealthChanged에 HandleHealthChanged 함수 바인딩
		playerStat->OnHealthChanged.AddDynamic(this, &UHpWidgetManageComponent::HandleHealthChanged);
	}
}

void UHpWidgetManageComponent::HandleHealthChanged(float curHp, float maxHp)
{
	PlayerHP->SetPlayerHP(Cast<ABG3PlayerCharacter>(GetOwner())->GetID(), curHp, maxHp);
}
