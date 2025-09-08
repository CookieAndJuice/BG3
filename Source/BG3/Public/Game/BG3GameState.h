// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BG3GameState.generated.h"

/**
 * 
 */
UCLASS()
class BG3_API ABG3GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void InitBG3GameState();

private:
	// round
	int Round = 1;
	int Turn = 1;

	// pawn numbers
	int Players = 0;
	int Enemies = 0;
};
