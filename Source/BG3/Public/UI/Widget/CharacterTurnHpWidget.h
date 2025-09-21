// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterTurnHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class BG3_API UCharacterTurnHpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UImage> CharacterImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UProgressBar> CharacterHPBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UButton> CharacterBtn;

	void InitializeWithCharacter(class ABaseCharacter* character);

	void SetPlayerHP(int32 curHp, int32 maxHp);

private:
	UPROPERTY()
	TObjectPtr<class UBG3GameManageSubsystem> GMSubsys;

	UPROPERTY()
	TWeakObjectPtr<class ABaseCharacter> OwnerCharacter;

	UFUNCTION()
	void CharacterBtnClicked();
};
