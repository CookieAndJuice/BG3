// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MainMenuGameMode.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/MainMenuWidget.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	ConstructorHelpers::FClassFinder<UMainMenuWidget> mainMenuRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_MainMenu.WBP_MainMenu_C'"));
	if (mainMenuRef.Succeeded())
	{
		MainMenuWidgetClass = mainMenuRef.Class;
	}
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
}
