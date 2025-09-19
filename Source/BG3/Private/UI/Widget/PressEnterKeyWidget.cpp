// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PressEnterKeyWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputMappingContext.h"


UPressEnterKeyWidget::UPressEnterKeyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprints/Input/MainMenu/IMC_MainMenu.IMC_MainMenu'"));
	if (IMCRef.Succeeded())
	{
		PressEnterKeyMappingContext = IMCRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> EnterKeyRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/MainMenu/IA_MainMenuEnter.IA_MainMenuEnter'"));
	if (EnterKeyRef.Succeeded())
	{
		PressEnterKeyAction = EnterKeyRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> SpaceBarKeyRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/MainMenu/IA_MainMenuSpaceBar.IA_MainMenuSpaceBar'"));
	if (SpaceBarKeyRef.Succeeded())
	{
		PressSpaceKeyAction = SpaceBarKeyRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> LMBRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/MainMenu/IA_MainMenuLMB.IA_MainMenuLMB'"));
	if (LMBRef.Succeeded())
	{
		ClickMouseAction = LMBRef.Object;
	}
}

void UPressEnterKeyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto* Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningPlayer()->GetLocalPlayer()))
	{
		Sub->AddMappingContext(PressEnterKeyMappingContext, 0);

		if (UEnhancedInputComponent* enhancedInput = CastChecked<UEnhancedInputComponent>(GetOwningPlayer()->InputComponent))
		{
			enhancedInput->BindAction(PressEnterKeyAction, ETriggerEvent::Started, this, &UPressEnterKeyWidget::PressEnterKey);
			enhancedInput->BindAction(ClickMouseAction, ETriggerEvent::Started, this, &UPressEnterKeyWidget::PressEnterKey);
		}
	}
}

void UPressEnterKeyWidget::PressEnterKey(const FInputActionValue& Value)
{
	OnProceedRequested.Broadcast();
}
