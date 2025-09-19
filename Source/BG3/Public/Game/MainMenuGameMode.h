// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BG3_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainMenuGameMode();

protected:
	virtual void BeginPlay() override;

public:	// UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMainMenuWidget> MainMenuWidget;
};
