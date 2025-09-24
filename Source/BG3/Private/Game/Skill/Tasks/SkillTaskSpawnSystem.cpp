#include "Game/Skill/Tasks/SkillTaskSpawnSystem.h"

#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Character/BG3PlayerCharacter.h"
#include "Controller/BG3GameModePlayerController.h"
#include "Game/BG3GameMode.h"
#include "Game/BG3GameState.h"
#include "Game/SkillExecutionSubsystem.h"

void USkillTaskSpawnSystem::Start(UObject* WorldContext, AActor* Caster, const USkillDefinition* Skill,
                                  const TArray<AActor*>& Targets)
{
	PRINTLOG(TEXT("SpawnSystem Start Called"));
	ABG3GameModePlayerController* PC = GetWorld()->GetFirstPlayerController<ABG3GameModePlayerController>();

	ABaseCharacter* Character = Cast<ABaseCharacter>(Caster);
	FVector TargetLocation = Targets[0]->GetActorLocation();

	PRINTLOG(TEXT("SpawnSystem Called"));
	PC->SpawnSystem(Character, TargetLocation);

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, [this, Targets]()
	{
		if (USkillExecutionSubsystem* Subsystem = GetWorld()->GetSubsystem<USkillExecutionSubsystem>())
		{
			ABG3GameState* GState = GetWorld()->GetGameState<ABG3GameState>();

			if (ABaseCharacter* Character = Cast<ABaseCharacter>(Targets[0]))
			{
				Character->SetIsHit(true);
			}
			
			Subsystem->FinalizeCastAfterExecutor(Targets, GState->GetCurrentRound());
		}
	}, 0.3f, false);
}

void USkillTaskSpawnSystem::Cancel()
{
	
}
