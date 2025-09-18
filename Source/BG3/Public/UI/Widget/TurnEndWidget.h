// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnEndWidget.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FOnTurnEndDelegate);

UCLASS()
class BG3_API UTurnEndWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	FOnTurnEndDelegate OnTurnEndDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UButton> TurnEndButton;

	UFUNCTION()
	void OnTurnEndButtonHovered();

	UFUNCTION()
	void OnTurnEndButtonPressed();

	UFUNCTION()
	void OnTurnEndButtonReleased();
	
	UFUNCTION()
	void OnTurnEndButtonClicked();
};
