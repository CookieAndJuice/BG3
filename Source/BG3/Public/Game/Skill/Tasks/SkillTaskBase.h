// Skill Task System - Base
//
// 목적
// - 스킬 실행 파이프라인(캐스팅 확정 이후)을 "작은 비동기 단계"로 쪼개 순차 오케스트레이션하기 위한 베이스 클래스입니다.
// - 각 Task는 시작(Start) → 완료(OnFinished) 또는 실패(OnFailed) 콜백으로 다음 Task로 제어를 넘깁니다.
// - 예: FaceTarget → MoveTo(사거리 진입) → PlayMontage(AnimNotify 대기) → (히트 시점) ApplyDamage
//
// 사용법
// - USkillCastExecutor가 Confirm 시점에 런타임 컨텍스트(거리, 타겟 모드)에 따라 Task 배열을 구성하고 Start부터 순차 실행합니다.
// - Task는 반드시 OnFinished/OnFailed를 통해 종료 신호를 보내야 하며, 내부에서 영속 대기 상태가 되지 않도록 주의합니다.
//
// 확장 포인트
// - 이동/애니메이션/발사체/라인트레이스/효과 적용 등 기능별 Task를 파생 클래스로 구현하세요.
// - 월드 컨텍스트/캐스터/스킬/타겟 배열을 필요 최소한으로 참조하고, 소유권은 Executor가 가집니다.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillTaskBase.generated.h"

DECLARE_DELEGATE(FOnTaskFinished);
DECLARE_DELEGATE_OneParam(FOnTaskFailed, FName /*Reason*/);

UCLASS(Abstract)
class BG3_API USkillTaskBase : public UObject
{
    GENERATED_BODY()
public:
    // Task 시작. 완료/실패 시 반드시 OnFinished/OnFailed 중 하나를 호출해야 다음 단계로 넘어갑니다.
    virtual void Start(UObject* WorldContext, AActor* Caster, const class USkillDefinition* Skill, const TArray<AActor*>& Targets) PURE_VIRTUAL(USkillTaskBase::Start, );
    virtual void Cancel() {}

    void Bind(const FOnTaskFinished& InOnFinished, const FOnTaskFailed& InOnFailed)
    {
        OnFinished = InOnFinished;
        OnFailed = InOnFailed;
    }

protected:
    FOnTaskFinished OnFinished;
    FOnTaskFailed OnFailed;
};
