// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/MainMenuWidgetController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/MainMenuWidget.h"
#include "UI/Widget/PressEnterKeyWidget.h"

AMainMenuWidgetController::AMainMenuWidgetController()
{
	ConstructorHelpers::FClassFinder<UPressEnterKeyWidget> pressEnterRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_PressEnterKey.WBP_PressEnterKey_C'"));
	if (pressEnterRef.Succeeded())
	{
		PressEnterKeyWidgetClass = pressEnterRef.Class;
	}
	
	ConstructorHelpers::FClassFinder<UMainMenuWidget> mainMenuRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_MainMenu.WBP_MainMenu_C'"));
	if (mainMenuRef.Succeeded())
	{
		MainMenuWidgetClass = mainMenuRef.Class;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> pressKeySoundRef(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sound/BGM/Title_OP__cut_34sec_.Title_OP__cut_34sec_'"));
	if (pressKeySoundRef.Succeeded())
	{
		PressKeySound = pressKeySoundRef.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> mainMenuSoundRef(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sound/BGM/03_NameInput.03_NameInput'"));
	if (mainMenuSoundRef.Succeeded())
	{
		MainMenuSound = mainMenuSoundRef.Object;
	}
}

void AMainMenuWidgetController::BeginPlay()
{
	Super::BeginPlay();

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();

	UGameplayStatics::PlaySound2D(GetWorld(), PressKeySound);

	SetShowMouseCursor(true);
}
