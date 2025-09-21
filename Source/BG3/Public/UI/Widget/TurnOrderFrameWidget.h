// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnOrderFrameWidget.generated.h"

/**
 * 
 */
UCLASS()
class BG3_API UTurnOrderFrameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	
	UPROPERTY()
	TArray<class UWidget*> WidgetToAddArray;

	UFUNCTION()
	void CreateFrameWidget();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UImage> SideDeco_L;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UImage> SideDeco_R;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UHorizontalBox> PortraitFrame;
};
