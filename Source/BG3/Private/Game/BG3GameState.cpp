// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BG3GameState.h"

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

int32 ABG3GameState::GetCurrentRound()
{
	return Round;
}

int32 ABG3GameState::GetCurrentTurn()
{
	return Turn;
}
