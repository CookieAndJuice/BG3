#include "Game/Skill/Tasks/SkillTaskMoveTo.h"

#include "BG3/BG3.h"
#include "AIController.h"
#include "AITypes.h"
#include "Data/SkillDefinition.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"

void USkillTaskMoveTo::Start(UObject* /*WorldContext*/, AActor* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets)
{
    if (!Caster || Targets.Num() == 0 || !IsValid(Targets[0]))
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("NoCaster"));
        return;
    }

    // 스킬 사거리(m)를 cm로 변환하여 수용 반경으로 사용. 값이 없으면 AcceptRadius 기본값 사용
    const float RangeCm = (Skill && Skill->Targeting.RangeMeters > 0.f) ? Skill->Targeting.RangeMeters * 50.f : AcceptRadius;

    const FVector From = Caster->GetActorLocation();
    const FVector To = Targets[0]->GetActorLocation();
    const float Dist = FVector::Dist(From, To);
    if (Dist <= RangeCm)
    {
        PRINTLOG(TEXT("[Task] MoveTo: already in range (%.0f cm <= %.0f cm)"), Dist, RangeCm);
        if (OnFinished.IsBound()) OnFinished.Execute();
        return;
    }

    PRINTLOG(TEXT("[Task] MoveTo: %s -> %s (range %.0f cm)"), *Caster->GetName(), *Targets[0]->GetName(), RangeCm);

    // Controller / NavSys diagnostics
    APawn* Pawn = Cast<APawn>(Caster);
    AAIController* AICon = Pawn ? Cast<AAIController>(Pawn->GetController()) : nullptr;
    PRINTLOG(TEXT("[Task] Ctrl=%s PawnCtrl=%s"),
        AICon ? *AICon->GetClass()->GetName() : TEXT("null"),
        Pawn && Pawn->GetController() ? *Pawn->GetController()->GetClass()->GetName() : TEXT("null"));

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Caster->GetWorld());
    PRINTLOG(TEXT("[Task] NavSys=%p"), NavSys);
    FNavLocation ProjTarget; bool bProjTarget = false;
    if (NavSys)
    {
        bProjTarget = NavSys->ProjectPointToNavigation(Targets[0]->GetActorLocation(), ProjTarget, FVector(100,100,200));
        PRINTLOG(TEXT("[Task] ProjectPointToNavigation %s -> %s"), bProjTarget ? TEXT("OK") : TEXT("FAIL"), bProjTarget ? *ProjTarget.Location.ToString() : TEXT("-"));
    }


    // 플레이어 PC가 Possess한 Pawn은 PlayerController이므로 AICon이 없을 수 있습니다.
    // 현 구현은 AI 전용이므로 이 경우 즉시 완료 처리(실제 이동 없음)합니다.
    if (!AICon)
    {
        PRINTLOG(TEXT("[Task] MoveTo: No AIController; finishing immediately"));
        if (OnFinished.IsBound()) OnFinished.Execute();
        return;
    }

    CachedController = AICon;
    // Clear any previous binding to avoid receiving stale completions
    AICon->ReceiveMoveCompleted.RemoveDynamic(this, &USkillTaskMoveTo::HandleMoveCompleted);
    LastRequestId = FAIRequestID();

    // Use FAIMoveRequest to obtain MoveId for filtering completed events
    FAIMoveRequest MoveReq;
    MoveReq.SetGoalActor(Targets[0]);
    MoveReq.SetAcceptanceRadius(RangeCm);
    MoveReq.SetUsePathfinding(true);
    MoveReq.SetAllowPartialPath(true);
    MoveReq.SetReachTestIncludesGoalRadius(true);
    MoveReq.SetReachTestIncludesAgentRadius(true);

    const FPathFollowingRequestResult Req = AICon->MoveTo(MoveReq);
    PRINTLOG(TEXT("[Task] MoveTo: RequestCode=%d MoveId=%d (0=Failed,1=AlreadyAtGoal,2=Success)"), (int32)Req.Code, (int32)Req.MoveId);
    if (Req.Code != EPathFollowingRequestResult::RequestSuccessful)
    {
        if (Req.Code == EPathFollowingRequestResult::AlreadyAtGoal)
        {
            if (OnFinished.IsBound()) OnFinished.Execute();
            return;
        }
        // Fallback: try MoveToLocation to approach point on ring around target
        if (NavSys)
        {
            const FVector CLoc = Caster->GetActorLocation();
            const FVector TLoc = Targets[0]->GetActorLocation();
            const FVector DirCT = (TLoc - CLoc).GetSafeNormal();
            const float Buffer = 50.f; // inside the range by small margin
            const float DesiredDist = FMath::Max(10.f, RangeCm - Buffer);
            const FVector Approach = TLoc - DirCT * DesiredDist;

            FNavLocation ProjApproach;
            bool bProjApproach = NavSys->ProjectPointToNavigation(Approach, ProjApproach, FVector(200,200,300));
            PRINTLOG(TEXT("[Task] Approach Project %s -> %s"), bProjApproach ? TEXT("OK") : TEXT("FAIL"), bProjApproach ? *ProjApproach.Location.ToString() : TEXT("-"));

            FAIMoveRequest MoveReqLoc;
            MoveReqLoc.SetGoalLocation(bProjApproach ? ProjApproach.Location : (bProjTarget ? ProjTarget.Location : TLoc));
            MoveReqLoc.SetAcceptanceRadius(RangeCm);
            MoveReqLoc.SetUsePathfinding(true);
            MoveReqLoc.SetAllowPartialPath(true);
            MoveReqLoc.SetReachTestIncludesGoalRadius(true);
            MoveReqLoc.SetReachTestIncludesAgentRadius(true);
            const FPathFollowingRequestResult Req2 = AICon->MoveTo(MoveReqLoc);
            PRINTLOG(TEXT("[Task] MoveToLocation Fallback: RequestCode=%d MoveId=%d"), (int32)Req2.Code, (int32)Req2.MoveId);
            if (Req2.Code != EPathFollowingRequestResult::RequestSuccessful)
            {
                if (Req2.Code == EPathFollowingRequestResult::AlreadyAtGoal)
                {
                    if (OnFinished.IsBound()) OnFinished.Execute();
                    return;
                }
                if (OnFailed.IsBound()) OnFailed.Execute(TEXT("MoveRequestFailed"));
                return;
            }
            LastRequestId = Req2.MoveId;
            AICon->ReceiveMoveCompleted.AddDynamic(this, &USkillTaskMoveTo::HandleMoveCompleted);
            return;
        }
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("MoveRequestFailed"));
        return;
    }
    LastRequestId = Req.MoveId;
    // Bind after we have a valid LastRequestId to ignore stale events
    AICon->ReceiveMoveCompleted.AddDynamic(this, &USkillTaskMoveTo::HandleMoveCompleted);
}

void USkillTaskMoveTo::Cancel()
{
    if (CachedController.IsValid())
    {
        CachedController->StopMovement();
        CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &USkillTaskMoveTo::HandleMoveCompleted);
    }
}

void USkillTaskMoveTo::HandleMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    if (!CachedController.IsValid())
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("ControllerLost"));
        return;
    }

    // Ignore completions that are not for our current request
    if (!LastRequestId.IsValid() || RequestID != LastRequestId)
    {
        return;
    }

    CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &USkillTaskMoveTo::HandleMoveCompleted);

    auto* PFC = CachedController->GetPathFollowingComponent();
    PRINTLOG(TEXT("[Task] MoveTo: Completed ReqId=%d Result=%d PFC=%p Status=%d"),
        (int32)RequestID, (int32)Result, PFC, PFC ? (int32)PFC->GetStatus() : -1);

    /*
    if (PFC && PFC->GetLastPath().IsValid())
    {
        PRINTLOG(TEXT("[Task] MoveTo: LastPath IsPartial=%d IsValid=%d"),
            (int32)PFC->GetLastPath()->IsPartial(), (int32)PFC->GetLastPath()->IsValid());
    }
    */
    
    if (Result == EPathFollowingResult::Success)
    {
        PRINTLOG(TEXT("[Task] MoveTo: Completed"));
        if (OnFinished.IsBound()) OnFinished.Execute();
    }
    else
    {
        PRINTLOG(TEXT("[Task] MoveTo: Failed (%d)"), (int32)Result);
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("MoveFailed"));
    }
}
