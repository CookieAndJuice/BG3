
#pragma once

#include "CoreMinimal.h"
#include "SkillTaskBase.h"
#include "SkillTaskEndTurn.generated.h"

UCLASS()
class BG3_API USkillTaskEndTurn : public USkillTaskBase
{
	GENERATED_BODY()

	virtual void Start(UObject* WorldContext, AActor* Caster, const class USkillDefinition* Skill, const TArray<AActor*>& Targets) override;
	virtual void Cancel() override;
};
