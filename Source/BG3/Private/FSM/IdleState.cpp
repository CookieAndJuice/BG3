// UObject-based IdleState implementation

#include "FSM/IdleState.h"
#include "BG3/BG3.h"
#include "Character/BG3EnemyCharacter.h"
#include "FSM/FSMComponent.h"

void UIdleState::Enter(class ABaseCharacter& Character)
{
	if (Character.IsA(ABG3EnemyCharacter::StaticClass()))
	{
		bCanUpdateBehavior = true;
	}
}

void UIdleState::Exit(class ABaseCharacter& Character)
{
	if (Character.IsA(ABG3EnemyCharacter::StaticClass()))
	{
		bCanUpdateBehavior = false;
	}
}

void UIdleState::UpdateBehavior()
{
	// Idle State Update
	if (bCanUpdateBehavior)
	{
		PRINTSTATELOG(TEXT("Idle Idle Idle State"));
	}
}
