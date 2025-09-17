#include "Character/Animation/BaseAnimInstance.h"
#include "Character/BaseCharacter.h"
#include "Game/Skill/Tasks/SkillTaskPlayMontage.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (const ABaseCharacter* Character = Cast<ABaseCharacter>(TryGetPawnOwner()))
    {
        const FVector Velocity = Character->GetVelocity();
        const FVector Forward = Character->GetActorForwardVector();
        const FVector Right = Character->GetActorRightVector();

        Speed = FVector::DotProduct(Velocity, Forward);
        Direction = FVector::DotProduct(Velocity, Right);

        if (const UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
        {
            IsInAir = MoveComp->IsFalling();
        }
    }
}

void UBaseAnimInstance::SetActiveMontageTask(USkillTaskPlayMontage* Task)
{
    ActiveMontageTask = Task;
}

USkillTaskPlayMontage* UBaseAnimInstance::GetActiveMontageTask() const
{
    return ActiveMontageTask.Get();
}

void UBaseAnimInstance::AnimNotify_Hit()
{
    OnHitNotifyBegin.Broadcast();

    if (ActiveMontageTask.IsValid())
    {
        ActiveMontageTask->HandleHitNotify();
    }
}
