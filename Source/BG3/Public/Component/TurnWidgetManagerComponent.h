// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurnWidgetManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BG3_API UTurnWidgetManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTurnWidgetManagerComponent();

	void AddWidgetToArray();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleHealthChanged(float curHp, float maxHp);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UCharacterTurnHpWidget> CharacterWidgetClass;
	
	UPROPERTY()
	TObjectPtr<class UCharacterTurnHpWidget> CharacterWidget;

	UPROPERTY()
	TObjectPtr<class UTurnOrderFrameWidget> TurnOrderFrame;
};
