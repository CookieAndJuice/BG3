// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuWidgetController.generated.h"

struct FInputActionValue;
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

	virtual void SetupInputComponent() override;

public: // input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> MainMenuIMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputAction> EnterKeyAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputAction> SpaceBarAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputAction> LMBAction;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Input)
	void OnSpaceBarClick(const FInputActionValue& value);
	UFUNCTION(BlueprintImplementableEvent, Category = Input)
	void OnEnterKeyClick(const FInputActionValue& value);
	
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

	UPROPERTY()
	TObjectPtr<class USoundBase> PressKeySound;
	
	UPROPERTY()
	TObjectPtr<class USoundBase> MainMenuSound;
};
