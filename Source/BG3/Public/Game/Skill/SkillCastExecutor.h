#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillCastExecutor.generated.h"

class ABaseCharacter;
class USkillDefinition;

UCLASS()
class BG3_API USkillCastExecutor : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION()
    void OnCastingStarted(ABaseCharacter* Caster, const USkillDefinition* Skill);

private:
    void ExecuteMove(ABaseCharacter* Caster, const USkillDefinition* Skill);
    void ExecuteProjectile(ABaseCharacter* Caster, const USkillDefinition* Skill);
};

