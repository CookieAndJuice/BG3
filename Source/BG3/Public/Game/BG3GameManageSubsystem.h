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
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:	// Initialize pawns
	void SpawnEnemies();

	void SpawnPlayers();

	void SortCombatPawns();

	void InitializeCamera();

public:	// Every Turn
	void BeginNextTurn();

private:
	UPROPERTY()
	TObjectPtr<class UInitialPlayerInfo> PlayerDataAsset;
	
public: // find pawns
	UFUNCTION(BlueprintCallable)
	ABaseCharacter* GetCurrentPawn();

	UPROPERTY(EditDefaultsOnly)
	TArray<FTurnData> CombatPawns;

private:	// find pawns
	int Index = 0;
};
