
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BG3GameManageSubsystem.generated.h"


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
	TObjectPtr<class UInitialCharacterInfo> PlayerDataAsset;

	UPROPERTY()
	TObjectPtr<class UInitialCharacterInfo> EnemyDataAsset;

	UPROPERTY()
	TObjectPtr<class ABG3GameMode> GM;
	
public: // Find Pawns
	UFUNCTION(BlueprintCallable)
	ABaseCharacter* GetCurrentPawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTurnData> CombatPawns;

private:	// Find Pawns
	int Index = 0;
};
