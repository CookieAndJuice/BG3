
#include "BG3/Public/Game/BG3GameMode.h"

#include "BG3/BG3.h"
#include "GameFramework/SpectatorPawn.h"
#include "Manager/BG3DiceManager.h"
#include "GameFramework/GameStateBase.h"

class ABG3GameModePlayerController;
struct FStatModifierData;

ABG3GameMode::ABG3GameMode()
{
	DefaultPawnClass = nullptr;
	ConstructorHelpers::FClassFinder<APlayerController> pcRef(TEXT("/Game/Blueprints/Game/BP_GamePlayerController.BP_GamePlayerController_C"));
	if (pcRef.Succeeded())
	{
		PlayerControllerClass = pcRef.Class;
	}
	ConstructorHelpers::FClassFinder<AGameStateBase> gsRef(TEXT("/Script/CoreUObject.Class'/Script/BG3.BG3GameState'"));
	if (gsRef.Succeeded())
	{
		GameStateClass = gsRef.Class;
	}
	
	Dice = CreateDefaultSubobject<UBG3DiceManager>(TEXT("Dice"));

	StatModifierByScore.SetNum(31);
	for (int s=1; s<=30; ++s)
	{
		StatModifierByScore[s] = FMath::FloorToInt((s - 10) / 2.0f);
	}

	
}

void ABG3GameMode::BeginPlay()
{
	Super::BeginPlay();
}

int32 ABG3GameMode::CalcInitiative(float Dex)
{
	int32 initiativeResult = 0;
	int32 diceValue = Dice->RollDice(1, 4);

	initiativeResult = diceValue + StatModifierByScore[Dex - 1];
	
	return initiativeResult;
}


