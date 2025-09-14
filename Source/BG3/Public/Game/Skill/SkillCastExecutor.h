#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillCastExecutor.generated.h"

class ABaseCharacter;
class USkillDefinition;
class USkillTaskBase;

UCLASS()
class BG3_API USkillCastExecutor : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION()
    void OnCastingStarted(ABaseCharacter* Caster, const USkillDefinition* Skill);

    UFUNCTION()
    void OnCastConfirmed(ABaseCharacter* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets, int32 CurrentRound);

private:
    void ExecuteMove(ABaseCharacter* Caster, const USkillDefinition* Skill);
    void ExecuteProjectile(ABaseCharacter* Caster, const USkillDefinition* Skill);

    void StartNext(UObject* WorldContext, AActor* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets);

    UPROPERTY()
    TArray<TObjectPtr<USkillTaskBase>> TaskQueue;

    // For finalize callback
    UPROPERTY()
    TWeakObjectPtr<ABaseCharacter> CachedCaster;
    UPROPERTY()
    TWeakObjectPtr<const USkillDefinition> CachedSkill;
    int32 CachedRound = 0;
};
