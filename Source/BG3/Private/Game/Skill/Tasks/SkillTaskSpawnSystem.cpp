#include "Game/Skill/Tasks/SkillTaskSpawnSystem.h"

#include "Character/BaseCharacter.h"
#include "Game/BG3GameMode.h"

void USkillTaskSpawnSystem::Start(UObject* WorldContext, AActor* Caster, const USkillDefinition* Skill,
                                  const TArray<AActor*>& Targets)
{
	ABG3GameMode* GM = GetWorld()->GetAuthGameMode<ABG3GameMode>();

	ABaseCharacter* Character = Cast<ABaseCharacter>(Caster);
	FVector TargetLocation = Targets[0]->GetActorLocation();
	GM->SpawnSystem(Character, TargetLocation);
}

void USkillTaskSpawnSystem::Cancel()
{
	
}
