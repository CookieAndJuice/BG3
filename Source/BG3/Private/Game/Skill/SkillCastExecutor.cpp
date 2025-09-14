#include "Game/Skill/SkillCastExecutor.h"
#include "Game/SkillExecutionSubsystem.h"
#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Data/SkillDefinition.h"
#include "Game/Skill/Tasks/SkillTaskBase.h"
#include "Game/Skill/Tasks/SkillTaskMoveTo.h"
#include "Game/Skill/Tasks/SkillTaskPlayMontage.h"
#include "Game/Skill/Tasks/SkillTaskSpawnProjectile.h"
#include "Game/Skill/Tasks/SkillTaskFaceTarget.h"

void USkillCastExecutor::OnCastingStarted(ABaseCharacter* Caster, const USkillDefinition* Skill)
{
    if (!Caster || !Skill)
    {
        return;
    }

    PRINTLOG(TEXT("CastingStarted: %s -> [%s]"), *Caster->GetName(), *Skill->Meta.DisplayName.ToString());
}

void USkillCastExecutor::OnCastConfirmed(ABaseCharacter* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets, int32 CurrentRound)
{
    if (!Caster || !Skill)
    {
        return;
    }

    PRINTLOG(TEXT("CastConfirmed: %s -> [%s], Targets: %d"), *Caster->GetName(), *Skill->Meta.DisplayName.ToString(), Targets.Num());

    TaskQueue.Reset();

    CachedCaster = Caster;
    CachedSkill = Skill;
    CachedRound = CurrentRound;

    // Skeleton plan: just demonstrate task queue and sequencing.
    // In future, branch by runtime context (distance, mode, etc.).
    // Desired melee plan: Face -> MoveTo(in range) -> Montage(wait Hit)
    USkillTaskFaceTarget* TFace = NewObject<USkillTaskFaceTarget>(this);
    USkillTaskMoveTo*     TMove = NewObject<USkillTaskMoveTo>(this);
    USkillTaskPlayMontage* TMont = NewObject<USkillTaskPlayMontage>(this);

    TMont->Montage = Skill->Meta.Montage;
    TMont->NotifyToWaitFor = Skill->Meta.HitNotifyName;

    TaskQueue.Add(TFace);
    TaskQueue.Add(TMove);
    TaskQueue.Add(TMont);

    StartNext(Caster, Caster, Skill, Targets);
}

void USkillCastExecutor::StartNext(UObject* WorldContext, AActor* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets)
{
    if (TaskQueue.Num() == 0)
    {
        PRINTLOG(TEXT("TaskQueue empty (skeleton complete)"));
        if (Caster && WorldContext)
        {
            if (UWorld* World = Caster->GetWorld())
            {
                if (class USkillExecutionSubsystem* SES = World->GetSubsystem<class USkillExecutionSubsystem>())
                {
                    // Apply damage now (e.g., after Hit notify) and finalize
                    TArray<AActor*> None;
                    if (CachedCaster.IsValid() && CachedSkill.IsValid())
                    {
                        SES->FinalizeCastAfterExecutor(None, CachedRound);
                    }
                }
            }
        }
        return;
    }

    USkillTaskBase* Task = TaskQueue[0];
    TaskQueue.RemoveAt(0);

    auto OnNext = FOnTaskFinished::CreateLambda([this, WorldContext, Caster, Skill, Targets]()
    {
        this->StartNext(WorldContext, Caster, Skill, Targets);
    });
    auto OnFail = FOnTaskFailed::CreateLambda([this](FName Reason)
    {
        PRINTLOG(TEXT("Task failed: %s"), *Reason.ToString());
        TaskQueue.Reset();
    });

    Task->Bind(OnNext, OnFail);
    Task->Start(WorldContext, Caster, Skill, Targets);
}

void USkillCastExecutor::ExecuteMove(ABaseCharacter* /*Caster*/, const USkillDefinition* /*Skill*/)
{
    // Placeholder for future move logic
}

void USkillCastExecutor::ExecuteProjectile(ABaseCharacter* /*Caster*/, const USkillDefinition* /*Skill*/)
{
    // Placeholder for future projectile logic
}
