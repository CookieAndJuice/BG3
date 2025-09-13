#include "Game/Skill/SkillCastExecutor.h"

#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Data/SkillDefinition.h"

void USkillCastExecutor::OnCastingStarted(ABaseCharacter* Caster, const USkillDefinition* Skill)
{
    if (!Caster || !Skill)
    {
        return;
    }

    PRINTLOG(TEXT("CastingStarted: %s -> [%s]"), *Caster->GetName(), *Skill->Meta.DisplayName.ToString());
}

void USkillCastExecutor::ExecuteMove(ABaseCharacter* /*Caster*/, const USkillDefinition* /*Skill*/)
{
    // Placeholder for future move logic
}

void USkillCastExecutor::ExecuteProjectile(ABaseCharacter* /*Caster*/, const USkillDefinition* /*Skill*/)
{
    // Placeholder for future projectile logic
}

