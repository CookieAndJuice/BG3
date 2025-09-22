
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BG3GameManageSubsystem.generated.h"


USTRUCT(Atomic, BlueprintType)
struct FTurnData
{
	GENERATED_USTRUCT_BODY()
public:
	int32 TurnOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class ABaseCharacter> TurnCharacter;
};

UCLASS()
class BG3_API UBG3GameManageSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UBG3GameManageSubsystem();

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
public:	// Initialize
	void SpawnPlayers();
	
	void SpawnEnemies();
	
	void InitializeGame();

public:	// Every Turn
	UFUNCTION()
	void BeginNextTurn();

private:
	UPROPERTY()
	bool bIsInAction = false;
	
private:// Initial Variables
	UPROPERTY()
	TObjectPtr<class UInitialCharacterInfo> PlayerDataAsset;

	UPROPERTY()
	TObjectPtr<class UInitialCharacterInfo> EnemyDataAsset;

	UPROPERTY()
	TObjectPtr<class ABG3GameMode> GM;

	UPROPERTY()
	TObjectPtr<class ABG3GameState> GState;

	UPROPERTY()
	TObjectPtr<class ABG3GameModePlayerController> GMPlayerController;

	UPROPERTY()
	TMap<int32, class ABG3PlayerCharacter*> PlayerMap;

	UPROPERTY()
	TMap<int32, class ABG3EnemyCharacter*> EnemyMap;
	
public: // Find Pawns
	UFUNCTION(BlueprintCallable)
	ABaseCharacter* GetCurrentPawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTurnData> CombatPawns;

	UFUNCTION(BlueprintCallable)
	class ABG3PlayerCharacter* GetPlayerFromID(int32 id);
	class ABG3EnemyCharacter* GetEnemyFromID(int32 id);

private:	// Find Pawns
	int Index = 0;

public: // Remove Pawns
	UFUNCTION()
	void RemoveCharacterFromCombatPawns(ABaseCharacter* InCharacter);

	int GetCombatPawnsIndex(ABaseCharacter* InCharacter);
};
