#include "Game/Skill/Tasks/SkillTaskPlayMontage.h"

#include "BG3/BG3.h"
#include "Animation/AnimInstance.h"
#include "Character/Animation/BaseAnimInstance.h"
#include "Character/BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/SkillDefinition.h"
#include "Game/SkillExecutionSubsystem.h"

void USkillTaskPlayMontage::Start(UObject* /*WorldContext*/, AActor* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets)
{
    CasterCharacter = Cast<ABaseCharacter>(Caster);
    WeakTargets.Reset();
    for (AActor* Target : Targets)
    {
        WeakTargets.Add(Target);
    }
    bHitApplied = false;

    if (!CasterCharacter.IsValid())
    {
        if (OnFailed.IsBound())
        {
            OnFailed.Execute(TEXT("NoCaster"));
        }
        return;
    }

    USkeletalMeshComponent* Mesh = CasterCharacter->GetMesh();
    UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;
    if (!AnimInstance)
    {
        if (OnFailed.IsBound())
        {
            OnFailed.Execute(TEXT("NoAnimInstance"));
        }
        return;
    }
    AnimInst = AnimInstance;

    UAnimMontage* MontageToPlay = Montage;
    if (!MontageToPlay && Skill)
    {
        MontageToPlay = Skill->GetMontageForMesh(Mesh);
    }

    if (!MontageToPlay)
    {
        if (OnFailed.IsBound())
        {
            OnFailed.Execute(TEXT("NoMontage"));
        }
        return;
    }

    Montage = MontageToPlay;

    if (Skill && Skill->Meta.HitNotifyName != NAME_None)
    {
        HitNotifyName = Skill->Meta.HitNotifyName;
    }

    if (UBaseAnimInstance* BaseAnim = Cast<UBaseAnimInstance>(AnimInstance))
    {
        BaseAnim->SetActiveMontageTask(this);
    }

    AnimInstance->OnMontageEnded.AddDynamic(this, &USkillTaskPlayMontage::OnMontageEnded);
    AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &USkillTaskPlayMontage::OnNotifyBegin);

    const float Length = AnimInstance->Montage_Play(MontageToPlay, 1.0f);
    if (Length <= 0.f)
    {
        ClearActiveTaskBinding();
        AnimInstance->OnMontageEnded.RemoveAll(this);
        AnimInstance->OnPlayMontageNotifyBegin.RemoveAll(this);
        if (OnFailed.IsBound())
        {
            OnFailed.Execute(TEXT("PlayFailed"));
        }
        return;
    }

    PRINTLOG(TEXT("[MontageTask] Play %s on %s"), *MontageToPlay->GetName(), *CasterCharacter->GetName());
}

void USkillTaskPlayMontage::Cancel()
{
    if (AnimInst.IsValid() && Montage)
    {
        ClearActiveTaskBinding();
        AnimInst->Montage_Stop(0.2f, Montage);
        AnimInst->OnMontageEnded.RemoveAll(this);
        AnimInst->OnPlayMontageNotifyBegin.RemoveAll(this);
    }
}

void USkillTaskPlayMontage::HandleHitNotify(FName TriggeredNotify)
{
    if (bHitApplied)
    {
        return;
    }

    const FName NotifyToLog = TriggeredNotify.IsNone() ? HitNotifyName : TriggeredNotify;
    if (!NotifyToLog.IsNone() && CasterCharacter.IsValid())
    {
        PRINTLOG(TEXT("[MontageTask] Notify %s on %s"), *NotifyToLog.ToString(), *CasterCharacter->GetName());
    }

    if (CasterCharacter.IsValid())
    {
        if (UWorld* World = CasterCharacter->GetWorld())
        {
            if (USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
            {
                TArray<AActor*> StrongTargets;
                StrongTargets.Reserve(WeakTargets.Num());
                for (const TWeakObjectPtr<AActor>& WeakTarget : WeakTargets)
                {
                    if (WeakTarget.IsValid())
                    {
                        StrongTargets.Add(WeakTarget.Get());
                        if (ABaseCharacter* Character = Cast<ABaseCharacter>(WeakTarget))
                        {
                            Character->SetIsHit(true);
                        }
                    }
                }
                SES->FinalizeCastAfterExecutor(StrongTargets, Round);
            }
        }
    }

    bHitApplied = true;

    if (OnFinished.IsBound())
    {
        OnFinished.Execute();
    }
}

void USkillTaskPlayMontage::OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted)
{
    if (!AnimInst.IsValid())
    {
        return;
    }

    ClearActiveTaskBinding();

    AnimInst->OnMontageEnded.RemoveAll(this);
    AnimInst->OnPlayMontageNotifyBegin.RemoveAll(this);

    if (bInterrupted && !bHitApplied)
    {
        if (OnFailed.IsBound())
        {
            OnFailed.Execute(TEXT("Interrupted"));
        }
        return;
    }

    if (!bHitApplied && OnFinished.IsBound())
    {
        OnFinished.Execute();
    }
}

void USkillTaskPlayMontage::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& /*Payload*/)
{
    if (NotifyName == HitNotifyName)
    {
        HandleHitNotify(NotifyName);
    }
}

void USkillTaskPlayMontage::ClearActiveTaskBinding()
{
    if (!AnimInst.IsValid())
    {
        return;
    }

    if (UBaseAnimInstance* BaseAnim = Cast<UBaseAnimInstance>(AnimInst.Get()))
    {
        if (BaseAnim->GetActiveMontageTask() == this)
        {
            BaseAnim->SetActiveMontageTask(nullptr);
        }
    }
}

