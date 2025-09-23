#include "Game/Skill/Tasks/SkillTaskMoveTo.h"

#include "BG3/BG3.h"
#include "AIController.h"
#include "AITypes.h"
#include "Character/BaseCharacter.h"
#include "Component/CharacterStatsComponent.h"
#include "Data/SkillDefinition.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Character/BG3EnemyCharacter.h"
#include "Game/BG3GameState.h"
#include "Game/SkillExecutionSubsystem.h"

namespace
{
bool FindGoalWithinBudget(const AActor& Caster, const FVector& Desired, float Budget, FVector& OutLocation, float& OutDistance)
{
    OutLocation = Desired;
    OutDistance = 0.f;

    if (Budget <= KINDA_SMALL_NUMBER)
    {
        return false;
    }

    UWorld* World = Caster.GetWorld();
    if (!World)
    {
        return false;
    }

    const FVector Start = Caster.GetActorLocation();
    if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World))
    {
        if (UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(World, Start, Desired))
        {
            const TArray<FVector>& Points = NavPath->PathPoints;
            if (Points.Num() >= 2)
            {
                float Accumulated = 0.f;
                for (int32 Index = 0; Index < Points.Num() - 1; ++Index)
                {
                    const FVector SegmentStart = Points[Index];
                    const FVector SegmentEnd   = Points[Index + 1];
                    const float SegmentLength  = FVector::Dist(SegmentStart, SegmentEnd);

                    if (Accumulated + SegmentLength < Budget - KINDA_SMALL_NUMBER)
                    {
                        Accumulated += SegmentLength;
                        continue;
                    }

                    const float Remaining = FMath::Clamp(Budget - Accumulated, 0.f, SegmentLength);
                    const FVector Direction = (SegmentEnd - SegmentStart).GetSafeNormal();
                    OutLocation = SegmentStart + Direction * Remaining;
                    OutDistance = Accumulated + Remaining;
                    return true;
                }

                OutLocation = Points.Last();
                OutDistance = Accumulated;
                return true;
            }
        }
    }

    const FVector Direction = (Desired - Start).GetSafeNormal();
    if (Direction.IsNearlyZero())
    {
        return false;
    }

    const float Straight = FVector::Dist(Start, Desired);
    OutDistance = FMath::Min(Budget, Straight);
    OutLocation = Start + Direction * OutDistance;
    return true;
}
}

void USkillTaskMoveTo::Start(UObject* /*WorldContext*/, AActor* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets)
{
    CachedStats.Reset();
    PlannedMoveDistance = 0.f;
    CachedStartLocation = FVector::ZeroVector;
    CachedController.Reset();
    LastRequestId = FAIRequestID();

    if (!Caster || Targets.Num() == 0 || !IsValid(Targets[0]))
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("NoCaster"));
        return;
    }

    const float RangeCm = (Skill && Skill->Targeting.RangeMeters > 0.f)
        ? Skill->Targeting.RangeMeters * 100.f
        : AcceptRadius;

    ABaseCharacter* CasterCharacter = Cast<ABaseCharacter>(Caster);
    if (!CasterCharacter || !CasterCharacter->Stats)
    {
        PRINTLOG(TEXT("[Task] MoveTo: caster lacks stats"));
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("NoStats"));
        return;
    }

    CachedStats = CasterCharacter->Stats;
    CachedStartLocation = CasterCharacter->GetActorLocation();
    CachedSkill = Skill;

    const float RemainingBudget = CachedStats->GetRemainingMoveDistance();
    if (RemainingBudget <= KINDA_SMALL_NUMBER)
    {
        PRINTLOG(TEXT("[Task] MoveTo: no remaining move distance"));
        if (OnFinished.IsBound()) OnFinished.Execute();
        CachedStats.Reset();
        return;
    }

    const FVector TargetLocation = Targets[0]->GetActorLocation();
    const float DistanceToTarget = FVector::Dist(CachedStartLocation, TargetLocation);
    const float NeededToEnterRange = FMath::Max(0.f, DistanceToTarget - RangeCm);
    const float MoveBudget = FMath::Min(RemainingBudget, NeededToEnterRange);

    if (MoveBudget <= KINDA_SMALL_NUMBER)
    {
        PRINTLOG(TEXT("[Task] MoveTo: already within range (dist=%.1f cm, range=%.1f cm)"), DistanceToTarget, RangeCm);
        if (OnFinished.IsBound()) OnFinished.Execute();
        CachedStats.Reset();
        return;
    }

    FVector GoalLocation = TargetLocation;
    float PlannedDistance = 0.f;
    if (!FindGoalWithinBudget(*CasterCharacter, TargetLocation, MoveBudget, GoalLocation, PlannedDistance))
    {
        PRINTLOG(TEXT("[Task] MoveTo: unable to find goal within budget (budget=%.1f cm)"), MoveBudget);
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("NoPath"));
        CachedStats.Reset();
        return;
    }

    PlannedMoveDistance = FMath::Clamp(PlannedDistance, 0.f, MoveBudget);
    if (PlannedMoveDistance <= KINDA_SMALL_NUMBER)
    {
        PRINTLOG(TEXT("[Task] MoveTo: no distance planned"));
        if (OnFinished.IsBound()) OnFinished.Execute();
        CachedStats.Reset();
        return;
    }

    APawn* Pawn = Cast<APawn>(Caster);
    AAIController* AICon = Pawn ? Cast<AAIController>(Pawn->GetController()) : nullptr;
    if (!AICon)
    {
        PRINTLOG(TEXT("[Task] MoveTo: No AIController; finishing immediately"));
        if (OnFinished.IsBound()) OnFinished.Execute();
        CachedStats.Reset();
        return;
    }

    CachedController = AICon;
    AICon->ReceiveMoveCompleted.RemoveDynamic(this, &USkillTaskMoveTo::HandleMoveCompleted);

    FAIMoveRequest MoveReq;
    MoveReq.SetGoalLocation(GoalLocation);
    MoveReq.SetAcceptanceRadius(FMath::Max(10.f, AcceptRadius));
    MoveReq.SetUsePathfinding(true);
    MoveReq.SetAllowPartialPath(false);
    MoveReq.SetProjectGoalLocation(true);
    MoveReq.SetReachTestIncludesGoalRadius(true);
    MoveReq.SetReachTestIncludesAgentRadius(true);

    const FPathFollowingRequestResult Req = AICon->MoveTo(MoveReq);
    PRINTLOG(TEXT("[Task] MoveTo: RequestCode=%d MoveId=%d Budget=%.1f Planned=%.1f"),
        (int32)Req.Code, (int32)Req.MoveId, MoveBudget, PlannedMoveDistance);

    if (Req.Code != EPathFollowingRequestResult::RequestSuccessful)
    {
        const float Travelled = FVector::Dist(CachedStartLocation, GoalLocation);
        if (CachedStats.IsValid() && Travelled > KINDA_SMALL_NUMBER)
        {
            const float Spent = FMath::Clamp(Travelled, 0.f, PlannedMoveDistance);
            CachedStats->ConsumeMoveDistance(Spent);
            PRINTLOG(TEXT("[Task] MoveTo: immediate spend %.1f, remaining %.1f"), Spent, CachedStats->GetRemainingMoveDistance());
        }

        CachedStats.Reset();
        PlannedMoveDistance = 0.f;
        CachedStartLocation = FVector::ZeroVector;
        CachedController.Reset();
        LastRequestId = FAIRequestID();

        if (Req.Code == EPathFollowingRequestResult::AlreadyAtGoal)
        {
            if (OnFinished.IsBound()) OnFinished.Execute();
        }
        else if (OnFailed.IsBound())
        {
            OnFailed.Execute(TEXT("MoveRequestFailed"));
        }
        return;
    }

    LastRequestId = Req.MoveId;
    AICon->ReceiveMoveCompleted.AddDynamic(this, &USkillTaskMoveTo::HandleMoveCompleted);
}

void USkillTaskMoveTo::Cancel()
{
    if (CachedController.IsValid())
    {
        CachedController->StopMovement();
        CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &USkillTaskMoveTo::HandleMoveCompleted);
        CachedController.Reset();
    }

    CachedStats.Reset();
    PlannedMoveDistance = 0.f;
    CachedStartLocation = FVector::ZeroVector;
    LastRequestId = FAIRequestID();
}

void USkillTaskMoveTo::HandleMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    AAIController* Controller = CachedController.Get();
    if (!Controller)
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("ControllerLost"));
        CachedStats.Reset();
        PlannedMoveDistance = 0.f;
        CachedStartLocation = FVector::ZeroVector;
        LastRequestId = FAIRequestID();
        return;
    }

    if (!LastRequestId.IsValid() || RequestID != LastRequestId)
    {
        return;
    }

    Controller->ReceiveMoveCompleted.RemoveDynamic(this, &USkillTaskMoveTo::HandleMoveCompleted);

    APawn* Pawn = Controller->GetPawn();
    const FVector CurrentLocation = Pawn ? Pawn->GetActorLocation() : CachedStartLocation;

    if (CachedStats.IsValid())
    {
        const float Travelled = FVector::Dist(CachedStartLocation, CurrentLocation);
        const float Spent = (PlannedMoveDistance > KINDA_SMALL_NUMBER)
            ? FMath::Clamp(Travelled, 0.f, PlannedMoveDistance)
            : Travelled;

        if (Spent > KINDA_SMALL_NUMBER)
        {
            CachedStats->ConsumeMoveDistance(Spent);
        }

        PRINTLOG(TEXT("[Task] MoveTo: completed result=%d spent=%.1f planned=%.1f remaining=%.1f"),
            (int32)Result, Spent, PlannedMoveDistance, CachedStats->GetRemainingMoveDistance());
    }

    if (Cast<ABG3EnemyCharacter>(CachedStats->GetOwner()))
    {
        if (CachedSkill && CachedSkill->Meta.ID == 888)
        {
            USkillExecutionSubsystem* Subsystem = GetWorld()->GetSubsystem<USkillExecutionSubsystem>();
            ABG3GameState* GS = GetWorld()->GetGameState<ABG3GameState>();
            TArray<AActor*> EmptyTargets;
            Subsystem->FinalizeCastAfterExecutor(EmptyTargets, GS->GetCurrentRound());
        }
    }

    CachedStats.Reset();
    PlannedMoveDistance = 0.f;
    CachedStartLocation = FVector::ZeroVector;
    CachedController.Reset();
    LastRequestId = FAIRequestID();

    if (Result == EPathFollowingResult::Success)
    {
        if (OnFinished.IsBound()) OnFinished.Execute();
    }
    else if (OnFailed.IsBound())
    {
        OnFailed.Execute(TEXT("MoveFailed"));
    }
}

