// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BG3GameManageSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class BG3_API UBG3GameManageSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public: // find pawns
	UFUNCTION(BlueprintCallable)
	APawn* GetCurrentPawn();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<AActor*> CombatPawns;

private:	// find pawns
	int Index = 0;
};
