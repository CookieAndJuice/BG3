// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MainMenuGameMode.h"

#include "UI/WidgetController/MainMenuWidgetController.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	ConstructorHelpers::FClassFinder<AMainMenuWidgetController> mainMenuController(TEXT("/Script/CoreUObject.Class'/Script/BG3.MainMenuWidgetController'"));
	if (mainMenuController.Succeeded())
	{
		PlayerControllerClass = mainMenuController.Class;
	}
}
