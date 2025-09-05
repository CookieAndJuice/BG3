// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BG3GameMode.generated.h"

/**
 * 
 */
UCLASS()
class BG3_API ABG3GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
public: // find pawns
	UFUNCTION(BlueprintCallable)
	APawn* GetCurrentPawn();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<AActor*> CombatPawns;

private:	// find pawns
	int Index = 0;
	
private:
	// round
	int Round = 0;
	int Turn = 0;

	// pawn numbers
	int Players = 0;
	int Enemies = 0;
};
