// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM/EnemyFSMComponent.h"

#include "BG3/BG3.h"
#include "Character/BG3EnemyCharacter.h"
#include "Character/BG3PlayerCharacter.h"
#include "Component/CharacterStatsComponent.h"
#include "Game/BG3GameManageSubsystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UEnemyFSMComponent::UEnemyFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UEnemyFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<ABG3EnemyCharacter>(GetOwner());
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
	case ECharacterState::Execute:
		ExecuteState();
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
	// select target

	// 1. max distance
	// if same 2. hp

	TArray<AActor*> playerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABG3PlayerCharacter::StaticClass(), playerArray);
	for (int32 i = 0; i < playerArray.Num(); i++)
	{
		ABG3PlayerCharacter* player = Cast<ABG3PlayerCharacter>(playerArray[i]);
		FVector dir = player->GetActorLocation() - me->GetActorLocation();
		float distance = dir.Size();

		if (distance > MaxDistance)
			continue;

		if (nullptr == target || Cast<ABG3PlayerCharacter>(player)->Stats->Health < Cast<ABG3PlayerCharacter>(target)->Stats->Health)
		{
			if (Cast<ABG3PlayerCharacter>(player)->Stats->Health != 0)
				target = player;
		}
	}
	
	// target distance
	
	// select action
	// if cannot do anything -> EndMyTurn()
}

void UEnemyFSMComponent::ExecuteState()
{
	PRINTSTATELOG(TEXT("Execute Execute Execute"));
}