#pragma once

#include "CoreMinimal.h"
// FaceTarget Task
//
// 목적
// - 캐스터를 첫 번째 타겟을 향해 즉시 회전(yaw만)시킵니다.
// - BG3 스타일 근접/원거리 공격에서 연출 자연스러움을 위해 선행 단계로 사용합니다.
//
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
