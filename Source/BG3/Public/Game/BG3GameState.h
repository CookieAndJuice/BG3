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
	void InitBG3GameState(int32 playerNum, int32 enemyNum);

	void UpdateTurn();

	void UpdateRound();

	void PrintCurrentState();

private:
	// round
	int32 Round = 1;
	int32 Turn = 1;

	// pawn numbers
	int32 Players = 0;
	int32 Enemies = 0;
};
