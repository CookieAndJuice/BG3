// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM/EnemyFSMComponent.h"

#include "BG3/BG3.h"
#include "Game/BG3GameManageSubsystem.h"


// Sets default values for this component's properties
UEnemyFSMComponent::UEnemyFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UEnemyFSMComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UEnemyFSMComponent::ChangeState(ECharacterState state)
{
	CurrentState = state;
}

void UEnemyFSMComponent::UpdateBehavior()
{
	switch (CurrentState)
	{
	case ECharacterState::Idle:
		IdleState();
		break;
	case ECharacterState::Plan:
		PlanState();
		break;
	case ECharacterState::Move:
		MoveState();
		break;
	case ECharacterState::Execute:
		ExecuteState();
		break;
	case ECharacterState::Hit:
		HitState();
		break;
	case ECharacterState::Die:
		DieState();
		break;
	}
}

void UEnemyFSMComponent::StartMyTurn()
{
	bIsMyTurn = true;
	ChangeState(ECharacterState::Plan);
}

void UEnemyFSMComponent::EndMyTurn()
{
	bIsMyTurn = false;
	ChangeState(ECharacterState::Idle);
	GetWorld()->GetSubsystem<UBG3GameManageSubsystem>()->BeginNextTurn();
}

void UEnemyFSMComponent::IdleState()
{
	PRINTSTATELOG(TEXT("Idle Idle Idle"));
}

void UEnemyFSMComponent::PlanState()
{
	PRINTSTATELOG(TEXT("Plan Plan Plan"));
}

void UEnemyFSMComponent::MoveState()
{
	PRINTSTATELOG(TEXT("Move Move Move"));
}

void UEnemyFSMComponent::ExecuteState()
{
	PRINTSTATELOG(TEXT("Execute Execute Execute"));
}

void UEnemyFSMComponent::HitState()
{
	PRINTSTATELOG(TEXT("Hit Hit Hit"));
}

void UEnemyFSMComponent::DieState()
{
	PRINTSTATELOG(TEXT("Die Die Die"));
}
