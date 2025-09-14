#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/Tasks/SkillTaskBase.h"

class USkillDefinition;
#include "SkillTaskFaceTarget.generated.h"

UCLASS()
class BG3_API USkillTaskFaceTarget : public USkillTaskBase
{
    GENERATED_BODY()
public:
    // Instantly rotate caster to face first target (yaw only)
    virtual void Start(UObject* WorldContext, AActor* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets) override;
};
