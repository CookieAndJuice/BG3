
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
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
public:	// Initialize
	void SpawnEnemies();

	void SpawnPlayers();

	void InitializeGameState();

public:	// Every Turn
	UFUNCTION()
	void BeginNextTurn();

private:
	UPROPERTY()
	bool bIsInAction = true;
	
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
	
public: // Find Pawns
	UFUNCTION(BlueprintCallable)
	ABaseCharacter* GetCurrentPawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTurnData> CombatPawns;

private:	// Find Pawns
	int Index = 0;
};
