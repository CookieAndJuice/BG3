#include "Game/Skill/Tasks/SkillTaskMoveTo.h"

#include "BG3/BG3.h"
#include "AIController.h"
#include "AITypes.h"
#include "Data/SkillDefinition.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"

void USkillTaskMoveTo::Start(UObject* /*WorldContext*/, AActor* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets)
{
    if (!Caster || Targets.Num() == 0 || !IsValid(Targets[0]))
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("NoCaster"));
        return;
    }

    const float RangeCm = (Skill && Skill->Targeting.RangeMeters > 0.f) ? Skill->Targeting.RangeMeters * 100.f : AcceptRadius;

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

    AAIController* AICon = nullptr;
    if (APawn* Pawn = Cast<APawn>(Caster))
    {
        AICon = Cast<AAIController>(Pawn->GetController());
    }

    if (!AICon)
    {
        PRINTLOG(TEXT("[Task] MoveTo: No AIController; finishing immediately"));
        if (OnFinished.IsBound()) OnFinished.Execute();
        return;
    }

    CachedController = AICon;
    AICon->ReceiveMoveCompleted.AddDynamic(this, &USkillTaskMoveTo::HandleMoveCompleted);

    FAIMoveRequest MoveReq;
    MoveReq.SetGoalActor(Targets[0]);
    MoveReq.SetAcceptanceRadius(RangeCm);
    //MoveReq.SetStopOnOverlap(true);

    LastRequestId = AICon->MoveTo(MoveReq);
    if (!LastRequestId.IsValid())
    {
        PRINTLOG(TEXT("[Task] MoveTo: Move request invalid"));
        AICon->ReceiveMoveCompleted.RemoveDynamic(this, &USkillTaskMoveTo::HandleMoveCompleted);
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("MoveRequestFailed"));
        return;
    }
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

    if (LastRequestId.IsValid() && RequestID != LastRequestId)
    {
        return;
    }

    CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &USkillTaskMoveTo::HandleMoveCompleted);

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
