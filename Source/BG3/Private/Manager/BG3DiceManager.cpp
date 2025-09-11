// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/BG3DiceManager.h"

UBG3DiceManager::UBG3DiceManager()
{
	
}

int32 UBG3DiceManager::RollDice(int32 diceNum, int32 maxDice)
{
	int32 result = FMath::RandRange(1, maxDice);
	// rollNum이 다수일 때 어떻게 해야 할까?
	return result;
}
