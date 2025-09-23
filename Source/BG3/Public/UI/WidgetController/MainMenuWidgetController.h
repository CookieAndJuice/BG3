// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class BG3_API AMainMenuWidgetController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainMenuWidgetController();
	
	virtual void BeginPlay() override;
	
private:
	// About Bool
	UPROPERTY()
	bool bTransitioningToMain = false;

	// About Widget Classes
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPressEnterKeyWidget> PressEnterKeyWidgetClass;

	UPROPERTY()
	TObjectPtr<class UPressEnterKeyWidget> PressEnterKeyWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMainMenuWidget> MainMenuWidget;

	// // About Press Enter Key Widget
	// UPROPERTY(EditAnywhere)
	// class UMediaPlayer* TouchPlayer1;
	//
	// UPROPERTY(EditAnywhere)
	// class UMediaPlayer* TouchPlayer2;
	//
	// UPROPERTY()
	// class UMediaTexture* TouchTexture1;
	//
	// UPROPERTY()
	// class UMediaTexture* TouchTexture2;
	//
	// UPROPERTY()
	// class UMediaSource* PressAnyKeyScreen1;
	//
	// UPROPERTY()
	// class UMediaSource* PressAnyKeyScreen2;

	// UPROPERTY()
	// TObjectPtr<class USoundBase*> MainMenuSound;
};
