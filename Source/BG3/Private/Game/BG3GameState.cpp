// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BG3GameState.h"

#include "BG3/BG3.h"

void ABG3GameState::InitBG3GameState(int32 playerNum, int32 enemyNum)
{
	Players = playerNum;
	Enemies = enemyNum;
}

void ABG3GameState::UpdateTurn()
{
	++Turn;
}

void ABG3GameState::UpdateRound()
{
	Turn = 1;
	++Round;
}

void ABG3GameState::PrintCurrentState()
{
	PRINTLOG(TEXT("Round %d | Turn %d"), Round , Turn);
	PRINTLOG(TEXT("Player %d | Enemy %d"), Players , Enemies);
}
