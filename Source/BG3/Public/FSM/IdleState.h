// Idle state implemented as UObject state

#pragma once

#include "CoreMinimal.h"
#include "FSM/FSMStateObject.h"
#include "IdleState.generated.h"

UCLASS(BlueprintType)
class BG3_API UIdleState : public UFSMStateObject
{
    GENERATED_BODY()

public:
    virtual void Enter(class ABaseCharacter& Character) override;
    virtual void Exit(class ABaseCharacter& Character) override;

    virtual void UpdateBehavior() override;
};

