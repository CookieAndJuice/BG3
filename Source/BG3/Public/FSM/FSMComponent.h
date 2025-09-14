// Copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/BG3EnemyCharacter.h"
#include "Components/ActorComponent.h"
#include "FSM/FSMStateObject.h"
#include "FSMComponent.generated.h"

// 전방 선언
class UFSMStateObject;
class ABaseCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BG3_API UFSMComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFSMComponent();

protected:
    virtual void BeginPlay() override;

public: // Change State & Update Behavior
    virtual void ChangeState(ABaseCharacter& character, ECharacterState state);

    virtual void UpdateBehavior();

public:
    // State StaticClasses & State Instances
    UPROPERTY(EditDefaultsOnly, Category = "FSM")
    TMap<ECharacterState, TSubclassOf<UFSMStateObject>> StateClasses;

    UPROPERTY()
    TMap<ECharacterState, UFSMStateObject*> States;

    // Current Enum State
    UPROPERTY(VisibleInstanceOnly, Category = "FSM")
    ECharacterState CurrentState = ECharacterState::None;

private:
    bool bIsTransitioning = false;

    UFSMStateObject* GetOrCreateState(ECharacterState State);
    bool TryTransition(ABaseCharacter& Character, ECharacterState Next);
};
