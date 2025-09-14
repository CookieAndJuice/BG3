// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM/FSMComponent.h"
#include "FSM/FSMStateObject.h"
#include "Character/BG3EnemyCharacter.h"


// Sets default values for this component's properties
UFSMComponent::UFSMComponent()
{
	// 상태 업데이트는 매니저가 수동으로 디스패치합니다.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UFSMComponent::BeginPlay()
{
    Super::BeginPlay();
}

UFSMStateObject* UFSMComponent::GetOrCreateState(ECharacterState State)
{
    // Find State -> Get
    if (UFSMStateObject** Found = States.Find(State))
    {
        return *Found;
    }

    // If Cannot Find State in "States Map" -> Find StateClass
    TSubclassOf<UFSMStateObject>* ClassPtr = StateClasses.Find(State);
    if (!ClassPtr || !ClassPtr->Get())
    {
        return nullptr;
    }

    // Create NewObject
    UFSMStateObject* Instance = NewObject<UFSMStateObject>(this, ClassPtr->Get());
    States.Add(State, Instance);
    return Instance;
}

bool UFSMComponent::TryTransition(ABaseCharacter& Character, ECharacterState Next)
{
    if (bIsTransitioning || Next == CurrentState)
    {
        return false;
    }

    UFSMStateObject* CurObject = GetOrCreateState(CurrentState);
    UFSMStateObject* NextObject = GetOrCreateState(Next);

    if (!NextObject)
    {
        return false;
    }

    // if ((CurObject && !CurObject->CanExit(Character)) || !NextObject->CanEnter(Character))
    // {
    //     return false;
    // }

    // Try Transition
    bIsTransitioning = true;
    // if (CurObject)
    // {
    //     CurObject->Exit(Character);
    // }
    CurrentState = Next;
    // NextObject->Enter(Character);
    bIsTransitioning = false;
    return true;
}

void UFSMComponent::ChangeState(ABaseCharacter& character, ECharacterState state)
{
    TryTransition(character, state);
}

void UFSMComponent::UpdateBehavior()
{
    if (UFSMStateObject** StateObject = States.Find(CurrentState))
    {
        (*StateObject)->UpdateBehavior();
    }
}
