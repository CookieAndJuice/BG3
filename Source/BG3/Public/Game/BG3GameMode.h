
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BG3GameMode.generated.h"

UENUM(BlueprintType)
enum class EResultState : uint8
{
	None,
	Player,
	Enemy
};

UCLASS()
class BG3_API ABG3GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABG3GameMode();
	
	virtual void BeginPlay() override;

private:// Stat Modifier
	const int32 MAXPOINT = 30;
	
	UPROPERTY()
	TArray<int32> StatModifierByScore;
	
public:	// Dice
	UPROPERTY()
	TObjectPtr<class UBG3DiceManager> Dice;

	int32 CalcInitiative(float Dex);

public:
	UFUNCTION(Category="Combat|UI")
	bool RequestUseSkill(class ABaseCharacter* Caster, int32 SkillID);
	void StopEnemies();
	bool IsEnemyWin();
	void FinalizeCast(const TArray<AActor*>& InTargets, int32 CurrentRound);

	
	EResultState DecideWhoWin();
};
