
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BG3GameMode.generated.h"

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
};
