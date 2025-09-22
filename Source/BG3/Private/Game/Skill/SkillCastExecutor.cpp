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

    for (auto TaskClass : Skill->Meta.Tasks)
    {
        if (!TaskClass) continue;

        USkillTaskBase* NewTask = NewObject<USkillTaskBase>(this, TaskClass);
        if (!NewTask) continue;

        // 태스크 타입 별 런타임 설정
        if (USkillTaskPlayMontage* MontageTask = Cast<USkillTaskPlayMontage>(NewTask))
        {
            MontageTask->Montage = Skill->GetMontageForMesh(Caster->GetMesh());
            MontageTask->HitNotifyName = Skill->SkillAssetSet.NotifyName;
            MontageTask->SetRound(CachedRound);
        }

        TaskQueue.Add(NewTask);
    }
    
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
        // Ensure subsystem state is released on failure to avoid Busy lock
        if (CachedCaster.IsValid())
        {
            if (UWorld* World = CachedCaster->GetWorld())
            {
                if (USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
                {
                    if (SES->IsBusy())
                    {
                        PRINTLOG(TEXT("[Executor] Cancel cast due to task failure"));
                        SES->CancelCast();
                    }
                }
            }
        }
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
