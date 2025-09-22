#include "Character/Animation/BaseAnimInstance.h"
#include "Character/BaseCharacter.h"
#include "Game/Skill/Tasks/SkillTaskPlayMontage.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Character) return;

    const FVector Velocity = Character->GetVelocity();
    const FVector Forward = Character->GetActorForwardVector();
    const FVector Right = Character->GetActorRightVector();

    Speed = FVector::DotProduct(Velocity, Forward);
    Direction = FVector::DotProduct(Velocity, Right);
    bIsHit = Character->GetIsHit();
    bIsDead = Character->GetIsDead();

    if (const UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
    {
        IsInAir = MoveComp->IsFalling();
    }
}

void UBaseAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();

    Character = Cast<ABaseCharacter>(TryGetPawnOwner());
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
        ActiveMontageTask->HandleHitNotify(FName(TEXT("Hit")));
    }
}

void UBaseAnimInstance::AnimNotify_HitEnd()
{
    Character->SetIsHit(false);
    bIsHit = false;
}

void UBaseAnimInstance::AnimNotify_Shoot()
{
    if (ActiveMontageTask.IsValid())
    {
        ActiveMontageTask->HandleShootNotify(FName(TEXT("Shoot")));
    }
}
