// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class BG3_API UPlayerHPWidget : public UUserWidget
{
	GENERATED_BODY()

public: // Bind UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UProgressBar> Player1HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UTextBlock> Player1HP;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UProgressBar> Player2HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UTextBlock> Player2HP;

	int32 maxHP1 = 0;
	int32 maxHP2 = 0;
	
	void InitUI(int32 hp1, int32 hp2);
	void SetPlayer1HP(int32 hp1);
	void SetPlayer2HP(int32 hp2);
};
