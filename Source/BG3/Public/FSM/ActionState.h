// Action state as UObject

#pragma once

#include "CoreMinimal.h"
#include "FSM/FSMStateObject.h"
#include "ActionState.generated.h"

UCLASS(BlueprintType)
class BG3_API UActionState : public UFSMStateObject
{
    GENERATED_BODY()
public:
    virtual void Enter(class ABaseCharacter& Character) override {}
    virtual void Exit(class ABaseCharacter& Character) override {}

    virtual void UpdateBehavior() override;
};

