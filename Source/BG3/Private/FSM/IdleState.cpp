// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM/IdleState.h"

#include "BG3/BG3.h"


// Sets default values for this component's properties
UIdleState::UIdleState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UIdleState::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UIdleState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UIdleState::UpdateBehavior()
{
	// Idle State Update
	PRINTLOG(TEXT("Idle Idle Idle State"));
	
}

