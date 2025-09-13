
// SkillExecutionSubsystem (간소 구현)
// - 상태 전이/예약/확정/적용을 단순 파이프라인으로 구성
// - 명중/세이브 등 고급 규칙은 생략, 타겟당 데미지 적용만 수행
#include "Game/SkillExecutionSubsystem.h"
#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Component/SkillBookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/BG3GameMode.h"
#include "Manager/BG3DiceManager.h"

USkillExecutionSubsystem::USkillExecutionSubsystem()
    : CastState(ECastState::Idle)
{
    // 초기 상태는 Idle
}

bool USkillExecutionSubsystem::RequestCast(ABaseCharacter* Caster, USkillDefinition* Skill)
{
    // 이미 다른 캐스트가 진행 중이면 거절
    if (CastState != ECastState::Idle)
    {
        PRINTLOG(TEXT("Busy"));
        return false;
    }
    
    // 유효성 검사(시전자, 스킬 보유 여부)
    if (!Caster || !Skill || !Caster->SkillBook->Skills.Contains(Skill) )
    {
        PRINTLOG(TEXT("No Caster or Skill"));
        return false;
    }
    
    bool OutResult;
    Caster->SkillBook->GetUsability(Skill, OutResult);
    if (!OutResult)
    {
        PRINTLOG(TEXT("GetUsability false"));
        return false; // 사용 불가 상태면 거절
    }

    // 리소스/행동 슬롯 예약
    if (Caster->SkillBook->ReserveUse(Skill))
    {
        // 컨텍스트 저장
        CurrentCaster = Caster;
        CurrentSkill = Skill;
        CurrentTargets.Reset();

        // 타게팅 단계 진입 알림
        CastState = ECastState::Targeting;
        PRINTLOG(TEXT("Reserve Success"));
        // TODO: SkillExecutor 구현 후 콜백함수 바인딩하기
        //CastingStarted.Execute(Caster, Skill);
        return true;
    }
    else
    {
        PRINTLOG(TEXT("Reserve Failed"));
        
        return false;
    }
}

void USkillExecutionSubsystem::CancelCast()
{
    // 대기 상태면 취소할 것이 없음
    if (CastState == ECastState::Idle)
    {
        return;
    }

    // 예약 환불 시도
    if (CurrentCaster.IsValid() && CurrentSkill.IsValid())
    {
        if (USkillBookComponent* SkillBook = CurrentCaster->FindComponentByClass<USkillBookComponent>())
        {
            SkillBook->CancelReservation(CurrentSkill.Get());
        }
    }

    // 상태 초기화 및 취소 알림
    ResetCast();
    CastState = ECastState::Canceled;
    CastingCanceled.ExecuteIfBound();
    CastState = ECastState::Idle;
}

void USkillExecutionSubsystem::SetTargets(const TArray<AActor*>& InTargets)
{
    // 타게팅 상태가 아니거나 컨텍스트가 없으면 무시
    if (CastState != ECastState::Targeting || !CurrentCaster.IsValid() || !CurrentSkill.IsValid())
    {
        return;
    }

    const USkillDefinition* Skill = CurrentSkill.Get();
    const ABaseCharacter* Caster = CurrentCaster.Get();

    TArray<AActor*> Filtered;
    Filtered.Reserve(InTargets.Num());

    // 사거리(m→cm 변환), 최대 타겟 제한
    const float RangeCm = (Skill->Targeting.RangeMeters <= 0.f ? 999999.f : Skill->Targeting.RangeMeters * 100.f);
    const int32 MaxTargets = Skill->Targeting.MaxTargets > 0 ? Skill->Targeting.MaxTargets : INT32_MAX;

    for (AActor* Target : InTargets)
    {
        // 자기 자신 제외, 유효성 검사
        if (!IsValid(Target) || Target == Caster)
        {
            continue;
        }

        if (!Caster)
        {
            continue;
        }

        // 거리 제한 체크
        const float Dist = FVector::Dist(Caster->GetActorLocation(), Target->GetActorLocation());
        if (Dist <= RangeCm)
        {
            Filtered.Add(Target);
            if (Filtered.Num() >= MaxTargets)
            {
                break;
            }
        }
    }

    // 내부 타겟 목록 갱신(약참조 저장)
    CurrentTargets.Reset();
    for (AActor* T : Filtered)
    {
        CurrentTargets.Add(T);
    }

    // UI 프리뷰 등 업데이트
    TargetsUpdated.ExecuteIfBound(Filtered);
}

bool USkillExecutionSubsystem::ConfirmAndExecute(int32 CurrentRound)
{
    // 타게팅 상태에서만 확정 가능
    if (CastState != ECastState::Targeting || !CurrentCaster.IsValid() || !CurrentSkill.IsValid())
    {
        PRINTLOG(TEXT("InvalidState"));
        return false;
    }

    ABG3GameMode* GM = GetWorld() ? Cast<ABG3GameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
    if (!GM)
    {
        PRINTLOG(TEXT("NoGameMode"));
        return false;
    }

    // 간단 해결 단계(고급 규칙 생략)
    CastState = ECastState::Resolving;

    // Simple resolution: roll damage dice once and apply to each target (no to-hit/save here for v1)
    USkillDefinition* Skill = CurrentSkill.Get();
    ABaseCharacter* Caster = CurrentCaster.Get();
    if (!Skill || !Caster)
    {
        PRINTLOG(TEXT("InvalidContext"));
        ResetCast();
        CastState = ECastState::Idle;
        return false;
    }

    // 안전한 주사위 파라미터 보정
    const int32 DiceNum = Skill->Damage.Dice.Num > 0 ? Skill->Damage.Dice.Num : 1;
    const int32 DiceSides = Skill->Damage.Dice.Sides > 0 ? Skill->Damage.Dice.Sides : 6;

    // 적용 단계로 전이
    CastState = ECastState::Applying;

    CurrentSkillResult = FSkillResult{};
    CurrentSkillResult.bSuccess = true;
    CurrentSkillResult.TotalDamage = 0;
    CurrentSkillResult.Affected.Reset();

    // Copy strong refs for Apply loop
    TArray<AActor*> Targets;
    for (const TWeakObjectPtr<AActor>& Weak : CurrentTargets)
    {
        if (Weak.IsValid())
        {
            Targets.Add(Weak.Get());
        }
    }

    for (AActor* Target : Targets)
    {
        // 각 타겟에 대해 주사위 굴림 후 데미지 적용
        const int32 Damage = GM->Dice ? GM->Dice->RollDice(DiceNum, DiceSides) : FMath::RandRange(DiceNum, DiceNum * DiceSides);
        CurrentSkillResult.TotalDamage += Damage;
        CurrentSkillResult.Affected.Add(Target);
        UGameplayStatics::ApplyDamage(Target, Damage, nullptr, Caster, UDamageType::StaticClass());
        Cast<ABaseCharacter>(Target);
    }

    // Commit cost/cooldown on success
    if (USkillBookComponent* SkillBook = Caster->FindComponentByClass<USkillBookComponent>())
    {
        SkillBook->CommitUse(Skill, CurrentRound);
    }

    // 완료 알림 후 상태 초기화
    CastState = ECastState::Completed;
    SkillResolved.ExecuteIfBound(CurrentSkillResult);

    ResetCast();
    CastState = ECastState::Idle;
    return true;
}

bool USkillExecutionSubsystem::IsBusy() const
{
    return CastState != ECastState::Idle;
}

ECastState USkillExecutionSubsystem::GetCastState() const
{
    return CastState;
}

void USkillExecutionSubsystem::OnClickInTargeting(FHitResult Hit)
{
    ETargetingMode Mode = CurrentSkill->Targeting.TargetingMode;

    if (Mode == ETargetingMode::Actor)
    {
        if (!Hit.GetActor()) return;
        if (!CurrentSkill->Targeting.bAllowSelfTarget && Hit.GetActor() == CurrentCaster) return;
        CurrentTargets.Add(Hit.GetActor());
    }
    
}

void USkillExecutionSubsystem::ResetCast()
{
    // 내부 버퍼와 참조를 모두 비움
    CurrentCaster = nullptr;
    CurrentSkill = nullptr;
    CurrentTargets.Reset();
    CurrentSkillResult = FSkillResult{};
}
