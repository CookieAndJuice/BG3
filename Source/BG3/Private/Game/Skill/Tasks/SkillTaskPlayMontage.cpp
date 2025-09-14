#include "Game/Skill/Tasks/SkillTaskPlayMontage.h"

#include "BG3/BG3.h"
#include "Animation/AnimInstance.h"
#include "Character/BaseCharacter.h"
#include "Character/Animation/BaseAnimInstance.h"
#include "GameFramework/Character.h"

void USkillTaskPlayMontage::Start(UObject* /*WorldContext*/, AActor* Caster, const USkillDefinition* /*Skill*/, const TArray<AActor*>& /*Targets*/)
{
    if (!Caster)
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("NoCaster"));
        return;
    }

    auto Character = Cast<ABaseCharacter>(Caster);
    auto AnimInst = Cast<UBaseAnimInstance>(Character->GetMesh()->GetAnimInstance());
    if (!AnimInst) return;

    PRINTLOG(TEXT("[Task] PlayMontage (skeleton) for %s"), *Caster->GetName());

    if (Montage)
    {
        AnimInst->Montage_Play(Montage.Get());
        AnimInst->OnHitNotifyBegin.AddUObject(this, &USkillTaskPlayMontage::OnNotifyBegin);
    }
    
    // Skeleton: no real montage; immediately finish as if notify arrived
    if (OnFinished.IsBound()) OnFinished.Execute();
}

void USkillTaskPlayMontage::Cancel()
{
}

void USkillTaskPlayMontage::OnNotifyBegin()
{
    PRINTLOG(TEXT("Hit Notify Received"));
}
