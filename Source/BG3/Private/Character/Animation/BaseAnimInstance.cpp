
#include "Character/Animation/BaseAnimInstance.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 속도 구하기
	// Pawn -> Player
	auto Character = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (Character)
	{
		FVector velocity = Character->GetVelocity();
		FVector forward = Character->GetActorForwardVector();
		FVector right = Character->GetActorRightVector();
		Speed = FVector::DotProduct(velocity, forward);
		Direction = FVector::DotProduct(velocity, right);

		auto cmp = Character->GetCharacterMovement();
		IsInAir = cmp->IsFalling();
	}
}

void UBaseAnimInstance::AnimNotify_Hit()
{
	
}
