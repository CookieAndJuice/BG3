#include "Game/Skill/Tasks/SkillTaskPlayMontage.h"

#include "BG3/BG3.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/BaseCharacter.h"
#include "Data/SkillDefinition.h"
#include "Game/SkillExecutionSubsystem.h"

void USkillTaskPlayMontage::Start(UObject* /*WorldContext*/, AActor* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets)
{
    CasterCharacter = Cast<ABaseCharacter>(Caster);
    WeakTargets.Reset();
    for (AActor* T : Targets)
    {
        WeakTargets.Add(T);
    }

    if (!CasterCharacter.IsValid())
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("NoCaster"));
        return;
    }

    USkeletalMeshComponent* Mesh = CasterCharacter->GetMesh();
    UAnimInstance* AI = Mesh ? Mesh->GetAnimInstance() : nullptr;
    if (!AI)
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("NoAnimInstance"));
        return;
    }
    AnimInst = AI;

    UAnimMontage* MontageToPlay = Montage;
    if (!MontageToPlay && Skill)
    {
        MontageToPlay = Skill->GetMontageForMesh(Mesh);
    }

    if (!MontageToPlay)
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("NoMontage"));
        return;
    }

    Montage = MontageToPlay;

    if (Skill && Skill->Meta.HitNotifyName != NAME_None)
    {
        HitNotifyName = Skill->Meta.HitNotifyName;
    }

    AI->OnMontageEnded.AddDynamic(this, &USkillTaskPlayMontage::OnMontageEnded);
    AI->OnPlayMontageNotifyBegin.AddDynamic(this, &USkillTaskPlayMontage::OnNotifyBegin);

    const float Len = AI->Montage_Play(MontageToPlay, 1.0f);
    if (Len <= 0.f)
    {
        AI->OnMontageEnded.RemoveAll(this);
        AI->OnPlayMontageNotifyBegin.RemoveAll(this);
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("PlayFailed"));
        return;
    }

    PRINTLOG(TEXT("[MontageTask] Play %s on %s"), *MontageToPlay->GetName(), *CasterCharacter->GetName());
}

void USkillTaskPlayMontage::Cancel()
{
    if (AnimInst.IsValid() && Montage)
    {
        AnimInst->Montage_Stop(0.2f, Montage);
        AnimInst->OnMontageEnded.RemoveAll(this);
        AnimInst->OnPlayMontageNotifyBegin.RemoveAll(this);
    }
}

void USkillTaskPlayMontage::OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted)
{
    if (!AnimInst.IsValid()) return;

    AnimInst->OnMontageEnded.RemoveAll(this);
    AnimInst->OnPlayMontageNotifyBegin.RemoveAll(this);

    if (bInterrupted && !bHitApplied)
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("Interrupted"));
        return;
    }

    if (!bHitApplied && OnFinished.IsBound())
    {
        OnFinished.Execute();
    }
}

void USkillTaskPlayMontage::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
    if (bHitApplied) return;
    //if (!Payload.MontageInstance) return;
    //if (Payload.MontageInstance->Montage != Montage) return;
    if (NotifyName != HitNotifyName) return;

    PRINTLOG(TEXT("[MontageTask] Notify %s on %s"), *NotifyName.ToString(), *CasterCharacter->GetName());

    if (CasterCharacter.IsValid())
    {
        if (UWorld* World = CasterCharacter->GetWorld())
        {
            if (USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
            {
                TArray<AActor*> StrongTargets;
                StrongTargets.Reserve(WeakTargets.Num());
                for (const TWeakObjectPtr<AActor>& W : WeakTargets)
                {
                    if (W.IsValid()) StrongTargets.Add(W.Get());
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
