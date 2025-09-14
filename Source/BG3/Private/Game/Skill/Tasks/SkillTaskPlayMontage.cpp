#include "Game/Skill/Tasks/SkillTaskPlayMontage.h"

#include "BG3/BG3.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

void USkillTaskPlayMontage::Start(UObject* /*WorldContext*/, AActor* Caster, const USkillDefinition* /*Skill*/, const TArray<AActor*>& /*Targets*/)
{
    if (!Caster)
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("NoCaster"));
        return;
    }

    PRINTLOG(TEXT("[Task] PlayMontage (skeleton) for %s"), *Caster->GetName());
    // Skeleton: no real montage; immediately finish as if notify arrived
    if (OnFinished.IsBound()) OnFinished.Execute();
}

void USkillTaskPlayMontage::Cancel()
{
}
