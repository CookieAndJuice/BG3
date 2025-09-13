
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SkillExecutionSubsystem.generated.h"

class ABG3GameMode;
class USkillDefinition;
class ABaseCharacter;
class UBG3GameManageSubsystem;
class USkillCastExecutor;

// 스킬 캐스팅 진행 상태(간소화 버전)
// Idle: 대기, Targeting: 대상 선택, Resolving/Applying: 계산/적용, Completed/Canceled: 종료 알림용

enum class ECastState : uint8
{
	Idle,
	Targeting,
	Resolving,
	Applying,
	Completed,
	Canceled
};

// 캐스팅 결과(간단 요약: 성공여부, 영향받은 액터, 총 데미지)
USTRUCT()
struct FSkillResult
{
    GENERATED_BODY()

    UPROPERTY()
    bool bSuccess = false;

    UPROPERTY()
    TArray<TObjectPtr<AActor>> Affected;

    UPROPERTY()
    int32 TotalDamage = 0;
};

// 이벤트 델리게이트(선택적으로 바인딩)
DECLARE_DELEGATE_TwoParams(FOnCastingStarted, ABaseCharacter* /*Caster*/, const USkillDefinition* /*Skill*/);
DECLARE_DELEGATE_OneParam(FOnTargetsUpdated, const TArray<AActor*>& /*Targets*/);
DECLARE_DELEGATE_OneParam(FOnSkillResolved, const FSkillResult& /*Result*/);
DECLARE_DELEGATE(FOnCastingCanceled);

/**
 * USkillExecutionSubsystem
 * - 스킬 사용 파이프라인을 간단히 오케스트레이션.
 * - 예약/커밋은 SkillBookComponent에 위임, 이 Subsystem은 상태/타겟/적용만 담당.
 */
UCLASS()
class BG3_API USkillExecutionSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // 기본 생성자(초기 상태 Idle)
    USkillExecutionSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    

    // 캐스트 시작 요청(검증+예약 → Targeting 진입)
    bool RequestCast(ABaseCharacter* Caster, USkillDefinition* Skill);
	
    // 캐스트 취소(예약 환불, 상태 초기화)
    void CancelCast();
	
    // 타겟 지정(사거리/최대수 제한 내부 검증 후 반영)
    void SetTargets(const TArray<AActor*>& InTargets);
	
    // 확정 및 실행(데미지 적용 후 커밋, 결과 브로드캐스트)
    bool ConfirmAndExecute(int32 CurrentRound);
	
    // 현재 바쁜지 여부(Idle이 아니면 true)
    bool IsBusy() const;
	
    // 현재 상태 조회
    ECastState GetCastState() const;

	void OnClickInTargeting(FHitResult Hit);

	

	/* 델리게이트 */
	
    FOnCastingStarted CastingStarted;
    FOnTargetsUpdated TargetsUpdated;
    FOnSkillResolved SkillResolved;
    FOnCastingCanceled CastingCanceled;

private:
    // 현재 시전자(약참조)
    UPROPERTY()
    TWeakObjectPtr<ABaseCharacter> CurrentCaster;

    // 현재 스킬(약참조)
    UPROPERTY()
    TWeakObjectPtr<USkillDefinition> CurrentSkill;

    // 현재 타겟들(약참조)
    UPROPERTY()
    TArray<TWeakObjectPtr<AActor>> CurrentTargets;

    // 상태 및 결과 버퍼
    ECastState CastState;
    FSkillResult CurrentSkillResult;

	
    // 외부 시스템 레퍼런스(필요 시 활용)
    TWeakObjectPtr<ABG3GameMode> GameMode;
    TWeakObjectPtr<UBG3GameManageSubsystem> TurnSubsystem;

    // Casting executor (presentation/aux logic)
    UPROPERTY()
    TObjectPtr<USkillCastExecutor> SkillExecutor;

    

    // 내부 상태 초기화(참조/결과 클리어)
    void ResetCast();
};
