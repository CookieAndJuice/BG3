// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerPortraitWidget.generated.h"

/**
 * 
 */
UCLASS()
class BG3_API UPlayerPortraitWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UImage> Portrait;
};
