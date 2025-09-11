// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BG3GameManageSubsystem.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FCurrentCharacterChanged, class ABaseCharacter*)

USTRUCT(Atomic, BlueprintType)
struct FTurnData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 TurnIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class ABaseCharacter> TurnCharacter;
};

UCLASS()
class BG3_API UBG3GameManageSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UBG3GameManageSubsystem();
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
public:	// Initialize Pawns
	void SpawnEnemies();

	void SpawnPlayers();

	void SortCombatPawns();

public:	// Every Turn
	void BeginNextTurn();

private:
	UPROPERTY()
	TObjectPtr<class UInitialPlayerInfo> PlayerDataAsset;

	UPROPERTY()
	TObjectPtr<class ABG3GameMode> GM;
	
public: // Find Pawns
	UFUNCTION(BlueprintCallable)
	ABaseCharacter* GetCurrentPawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTurnData> CombatPawns;

private:	// Find Pawns
	int Index = 0;
	int tempSpawnIndex = 0;	// 나중에 주사위 선제권 값으로 바꿀 것
};
