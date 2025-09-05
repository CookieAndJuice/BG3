// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BG3GameManageSubsystem.h"

APawn* UBG3GameManageSubsystem::GetCurrentPawn()
{
	APawn* pawn = CastChecked<APawn>(CombatPawns[Index]);
	Index += Index % CombatPawns.Num();
	return pawn;
}