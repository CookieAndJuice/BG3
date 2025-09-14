// UObject-based MoveState implementation

#include "FSM/MoveState.h"
#include "BG3/BG3.h"
#include "Character/BG3EnemyCharacter.h"
#include "FSM/FSMComponent.h"

void UMoveState::Enter(class ABaseCharacter& Character)
{
    if (Character.IsA(ABG3EnemyCharacter::StaticClass()))
    {
        bCanUpdateBehavior = true;
    }
}

void UMoveState::Exit(class ABaseCharacter& Character)
{
    if (Character.IsA(ABG3EnemyCharacter::StaticClass()))
    {
        bCanUpdateBehavior = false;
    }
}

void UMoveState::UpdateBehavior()
{
    if (bCanUpdateBehavior)
        PRINTLOG(TEXT("Move Move Move State"));
}
