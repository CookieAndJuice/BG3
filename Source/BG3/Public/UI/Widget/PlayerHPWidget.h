// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPWidget.generated.h"

/**
 * 
 */
USTRUCT()
struct FPlayerHPWidgetData
{
	GENERATED_BODY()
	UPROPERTY()
	TObjectPtr<class UTextBlock> HpText;
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpBar;
};

UCLASS()
class BG3_API UPlayerHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
public: // Bind UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UProgressBar> Player1HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UTextBlock> Player1HP;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UProgressBar> Player2HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UTextBlock> Player2HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UButton> Player1Btn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UButton> Player2Btn;
	
public:
	void SetPlayerHP(int32 id, int32 curHp, int32 maxHp);

private:
	UFUNCTION()
	void OnPlayer1BtnClicked();

	UFUNCTION()
	void OnPlayer2BtnClicked();
	
	UPROPERTY()
	TMap<int32, FPlayerHPWidgetData> SlotMap;

	UPROPERTY()
	TObjectPtr<class UBG3GameManageSubsystem> GMSubsys;
};
