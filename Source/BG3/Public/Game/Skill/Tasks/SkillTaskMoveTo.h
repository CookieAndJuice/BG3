// MoveTo Task
//
// 목적
// - 캐스터가 목표 대상(첫 타겟)까지 "스킬 거리"를 고려해 네비게이션 이동을 요청합니다.
// - 거리가 짧으면 즉시 완료, 거리가 멀면 AI MoveTo로 이동 후 완료/실패 콜백을 호출합니다.
//
// 주의
// - 현재 구현은 AIController 기반입니다. 플레이어가 Possess한 Pawn은 PlayerController이므로
//   AAIController가 없을 때 즉시 완료 처리합니다(실제 이동 없음). 플레이어 이동이 필요하면
//   별도 분기(예: SimpleMoveToActor(PC, Target))가 필요합니다.
#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/Tasks/SkillTaskBase.h"
#include "AIController.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "SkillTaskMoveTo.generated.h"

class UCharacterStatsComponent;

UCLASS()
class BG3_API USkillTaskMoveTo : public USkillTaskBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move")
    float AcceptRadius = 70.f; // cm

    // WorldContext: 보통 Caster 또는 Subsystem
    // Caster: 이동 주체
    // Skill: 타겟팅 거리(RangeMeters)를 이동 범위로 사용
    // Targets: 첫 요소를 목표로 사용
    virtual void Start(UObject* WorldContext, AActor* Caster, const class USkillDefinition* Skill, const TArray<AActor*>& Targets) override;
    virtual void Cancel() override;

private:
    UFUNCTION()
    void HandleMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

    UPROPERTY()
    TWeakObjectPtr<class AAIController> CachedController;

    UPROPERTY()
    TWeakObjectPtr<UCharacterStatsComponent> CachedStats;

    FVector CachedStartLocation = FVector::ZeroVector;
    float PlannedMoveDistance = 0.f;

    // Track last move request to filter callbacks
    FAIRequestID LastRequestId;
};
