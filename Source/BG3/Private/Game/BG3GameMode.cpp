// Fill out your copyright notice in the Description page of Project Settings.


#include "BG3/Public/Game/BG3GameMode.h"

#include "Kismet/GameplayStatics.h"

void ABG3GameMode::BeginPlay()
{
	Super::BeginPlay();
}

APawn* ABG3GameMode::GetCurrentPawn()
{
	APawn* pawn = CastChecked<APawn>(CombatPawns[Index]);
	Index += Index % CombatPawns.Num();
	return pawn;
}
