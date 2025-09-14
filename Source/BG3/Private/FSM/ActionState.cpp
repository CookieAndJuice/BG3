// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM/ActionState.h"

#include "BG3/BG3.h"


// Sets default values for this component's properties
UActionState::UActionState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActionState::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActionState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActionState::ChangeState(ABaseCharacter& character, ECharacterState state)
{
	Super::ChangeState(character, state);
}

void UActionState::UpdateBehavior()
{
	Super::UpdateBehavior();

	PRINTLOG(TEXT("Action Action Action State"));
}

