
#include "BG3/Public/Game/BG3GameMode.h"

#include "BG3/BG3.h"
#include "Data/SkillDefinition.h"
#include "Game/SkillExecutionSubsystem.h"
#include "Manager/BG3DiceManager.h"
#include "GameFramework/GameStateBase.h"
#include "Character/BaseCharacter.h"
#include "Character/BG3EnemyCharacter.h"
#include "Character/BG3PlayerCharacter.h"
#include "Component/SkillBookComponent.h"
#include "Game/BG3GameManageSubsystem.h"

class ABG3GameModePlayerController;
struct FStatModifierData;

ABG3GameMode::ABG3GameMode()
{
	// ConstructorHelpers::FClassFinder<ABG3GameCamera> cameraRef(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Actor/BP_GameCamera.BP_GameCamera_C'"));
	// if (cameraRef.Succeeded())
	// {
	// 	DefaultPawnClass = cameraRef.Class;
	// }
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

bool ABG3GameMode::RequestUseSkill(ABaseCharacter* Caster, int32 SkillID)
{
	if (!Caster->SkillBook)
	{
		return false;
	}
	for (USkillDefinition* Def : Caster->SkillBook->Skills)
	{
		if (Def && Def->Meta.ID == SkillID)
		{
			if (UWorld* World = GetWorld())
			{
				if (USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
				{
					if (SES->RequestCast(Caster, Def))
					{
						PRINTLOG(TEXT("Skill cast started (targeting)"));
						return true;
					}
					else
					{
						PRINTLOG(TEXT("Skill cast request failed"));
						return false;
					}
				}
			}
		}
	}

	return false;
}

EResultState ABG3GameMode::DecideWhoWin()
{
	EResultState result = EResultState::None;

	UBG3GameManageSubsystem* Subsystem = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>();
	if (!Subsystem) return result;

	int playerCnt = 0, enemyCnt = 0;
	
	for (auto character : Subsystem->CombatPawns)
	{
		if (character.TurnCharacter->IsA(ABG3EnemyCharacter::StaticClass()))
		{
			enemyCnt++;
		}
		else if (character.TurnCharacter->IsA(ABG3PlayerCharacter::StaticClass()))
		{
			playerCnt++;
		}
	}

	if (enemyCnt == 0) return EResultState::Player;
	else if (playerCnt == 0) return EResultState::Enemy;
	
	return result;
}
