// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NewGameBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::StartGame);
	QuitGameBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
}

void UMainMenuWidget::StartGame()
{
	UGameplayStatics::OpenLevel(this, FName("Dream_Island"));
}

void UMainMenuWidget::QuitGame()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
	}
}
