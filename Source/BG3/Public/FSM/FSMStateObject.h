// FSM State base as UObject

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Character/BaseCharacter.h"
#include "FSMStateObject.generated.h"

UCLASS(Abstract, BlueprintType)
class BG3_API UFSMStateObject : public UObject
{
    GENERATED_BODY()
public:
    // 진입/이탈 훅
    virtual void Enter(ABaseCharacter& Character) {}
    virtual void Exit(ABaseCharacter& Character) {}

    // Update Behavior
    virtual void UpdateBehavior() PURE_VIRTUAL(UFSMStateObject::UpdateBehavior, );

    // 전이 가드/이벤트 (선택적)
    virtual bool CanEnter(const ABaseCharacter& Character) const { return true; }
    virtual bool CanExit(const ABaseCharacter& Character) const { return true; }
    virtual void OnEvent(FName /*EventId*/, ABaseCharacter& /*Character*/) {}
};

