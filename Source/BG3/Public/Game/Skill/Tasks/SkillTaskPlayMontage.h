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

class ABaseCharacter;

UCLASS()
class BG3_API USkillTaskPlayMontage : public USkillTaskBase
{
    GENERATED_BODY()
public:
    // 재생할 몽타주(필수)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
    UAnimMontage* Montage = nullptr;

    // 맞는 시점으로 사용할 AnimNotify 이름(기본: "Hit")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
    FName HitNotifyName = FName(TEXT("Hit"));

    UFUNCTION(BlueprintCallable, Category="Montage")
    void SetRound(int32 InRound) { Round = InRound; }

    virtual void Start(UObject* WorldContext, AActor* Caster, const class USkillDefinition* Skill, const TArray<AActor*>& Targets) override;
    virtual void Cancel() override;

private:
    UFUNCTION()
    void OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted);

    UFUNCTION()
    void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);

private:
    UPROPERTY()
    TWeakObjectPtr<ABaseCharacter> CasterCharacter;

    UPROPERTY()
    TWeakObjectPtr<UAnimInstance> AnimInst;

    UPROPERTY()
    TArray<TWeakObjectPtr<AActor>> WeakTargets;

    UPROPERTY()
    int32 Round = 0;

    bool bHitApplied = false;
};
