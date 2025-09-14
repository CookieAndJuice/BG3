// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM/MoveState.h"

#include "BG3/BG3.h"


// Sets default values for this component's properties
UMoveState::UMoveState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveState::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMoveState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMoveState::ChangeState(ABaseCharacter& character, ECharacterState state)
{
	Super::ChangeState(character, state);
}

void UMoveState::UpdateBehavior()
{
	Super::UpdateBehavior();

	PRINTLOG(TEXT("Move Move Move State"));
}

