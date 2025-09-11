// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BG3DiceManager.generated.h"

/**
 * 
 */
UCLASS()
class BG3_API UBG3DiceManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UBG3DiceManager();
	
	int32 RollDice(int32 diceNum, int32 maxDice);
};
