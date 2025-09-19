// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/MainMenuWidgetController.h"

#include "Blueprint/UserWidget.h"
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
}

void AMainMenuWidgetController::BeginPlay()
{
	Super::BeginPlay();

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
}
