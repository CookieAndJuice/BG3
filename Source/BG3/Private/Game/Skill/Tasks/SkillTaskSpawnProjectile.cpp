#include "Game/Skill/Tasks/SkillTaskSpawnProjectile.h"

#include "BG3/BG3.h"
#include "Engine/World.h"

void USkillTaskSpawnProjectile::Start(UObject* WorldContext, AActor* Caster, const USkillDefinition* /*Skill*/, const TArray<AActor*>& Targets)
{
    if (!WorldContext || !Caster)
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("InvalidContext"));
        return;
    }

    PRINTLOG(TEXT("[Task] SpawnProjectile (skeleton) by %s"), *Caster->GetName());
    // Skeleton only: do not actually spawn; finish immediately.
    if (OnFinished.IsBound()) OnFinished.Execute();
}

void USkillTaskSpawnProjectile::Cancel()
{
}
