#include "Game/Skill/Tasks/SkillTaskFaceTarget.h"

#include "BG3/BG3.h"
#include "GameFramework/Actor.h"

void USkillTaskFaceTarget::Start(UObject* /*WorldContext*/, AActor* Caster, const USkillDefinition* /*Skill*/, const TArray<AActor*>& Targets)
{
    if (!Caster || Targets.Num() == 0 || !IsValid(Targets[0]))
    {
        if (OnFinished.IsBound()) OnFinished.Execute();
        return;
    }

    const FVector ToTarget = Targets[0]->GetActorLocation() - Caster->GetActorLocation();
    FRotator NewRot = ToTarget.Rotation();
    NewRot.Pitch = 0.f; // yaw only
    NewRot.Roll = 0.f;
    Caster->SetActorRotation(NewRot);

    PRINTLOG(TEXT("[Task] FaceTarget: %s -> %s"), *Caster->GetName(), *Targets[0]->GetName());
    if (OnFinished.IsBound()) OnFinished.Execute();
}

