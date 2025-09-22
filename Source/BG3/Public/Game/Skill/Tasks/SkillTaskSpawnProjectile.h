// SpawnProjectile Task
//
// 목적
// - 발사체 액터를 스폰하고, OnHit(충돌) 이벤트를 통해 후속 단계(데미지 적용)로 이어집니다.
// - BG3 스타일 투사체형 스킬(파이어볼트 등)을 위해 사용합니다.
//
// 현재 상태
// - 스켈레톤 구현: 스폰/OnHit 대기 없이 즉시 완료합니다.
// - 추후: ProjectileClass, 속도/중력/임팩트 처리, OnHit에서 Executor 콜백 호출로 확장하세요.
#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/Tasks/SkillTaskBase.h"
#include "SkillTaskSpawnProjectile.generated.h"

UCLASS()
class BG3_API USkillTaskSpawnProjectile : public USkillTaskBase
{
    GENERATED_BODY()
public:

    virtual void Start(UObject* WorldContext, AActor* Caster, const class USkillDefinition* Skill, const TArray<AActor*>& Targets) override;
    virtual void Cancel() override;
};
