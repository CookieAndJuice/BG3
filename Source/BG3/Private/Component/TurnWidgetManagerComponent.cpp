// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TurnWidgetManagerComponent.h"

#include "Character/BG3PlayerCharacter.h"
#include "Controller/BG3GameModePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/CharacterTurnHpWidget.h"
#include "Component/CharacterStatsComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "UI/Widget/OverlayWidget.h"
#include "UI/Widget/TurnOrderFrameWidget.h"


class UCharacterStatsComponent;
// Sets default values for this component's properties
UTurnWidgetManagerComponent::UTurnWidgetManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FClassFinder<UCharacterTurnHpWidget> charWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_CharacterTurnHP.WBP_CharacterTurnHP_C'"));
	if (charWidgetRef.Succeeded())
	{
		CharacterWidgetClass = charWidgetRef.Class;
	}
}

void UTurnWidgetManagerComponent::AddWidgetToArray()
{
	if (auto* PC = Cast<ABG3GameModePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (UOverlayWidget* Overlay = PC->OverlayWidget)
		{
			TurnOrderFrame = Overlay->GetTurnOrderFrame();
		}
	}
	CharacterWidget = CreateWidget<UCharacterTurnHpWidget>(GetWorld(), CharacterWidgetClass);
	CharacterWidget->InitializeWithCharacter(Cast<ABaseCharacter>(GetOwner()));

	if (UTexture2D* portrait = Cast<ABaseCharacter>(GetOwner())->GetSmallPortrait())
	{
		CharacterWidget->CharacterImage->SetBrushFromTexture(portrait);
	}
	TurnOrderFrame->WidgetToAddArray.Add(CharacterWidget);
}

// Called when the game starts
void UTurnWidgetManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (UCharacterStatsComponent* characterStat = Cast<ABaseCharacter>(GetOwner())->Stats)
	{
		HandleHealthChanged(characterStat->GetHealth(), characterStat->GetMaxHealth());
	
		// Stat의 OnHealthChanged에 HandleHealthChanged 함수 바인딩
		characterStat->OnHealthChanged.AddDynamic(this, &UTurnWidgetManagerComponent::HandleHealthChanged);
	}
}

void UTurnWidgetManagerComponent::HandleHealthChanged(float curHp, float maxHp)
{
	CharacterWidget->SetPlayerHP(curHp, maxHp);
}
