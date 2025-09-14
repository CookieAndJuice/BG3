// PlayMontage Task
//
// 목적
// - 캐스터에 지정된 AnimMontage를 재생하고, 필요 시 특정 AnimNotify(예: "Hit")를 대기합니다.
// - 근접 공격의 실제 판정(데미지 적용) 타이밍을 AnimNotify와 동기화하기 위해 사용합니다.
//
// 현재 상태
// - 스켈레톤 구현: 실제 재생/노티파이 대기 없이 즉시 완료합니다.
// - 추후: SkillDefinition에 몽타주/노티파이 이름을 추가하고 대기 로직을 연결하세요.
#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/Tasks/SkillTaskBase.h"
#include "Animation/AnimMontage.h"
#include "SkillTaskPlayMontage.generated.h"

UCLASS()
class BG3_API USkillTaskPlayMontage : public USkillTaskBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
    TObjectPtr<UAnimMontage> Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
    FName NotifyToWaitFor = NAME_None; // Optional: wait specific notify

    virtual void Start(UObject* WorldContext, AActor* Caster, const class USkillDefinition* Skill, const TArray<AActor*>& Targets) override;
    virtual void Cancel() override;
    void OnNotifyBegin();
};
