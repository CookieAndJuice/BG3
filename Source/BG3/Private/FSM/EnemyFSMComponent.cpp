// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM/EnemyFSMComponent.h"


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

void UEnemyFSMComponent::StartFSM()
{
	auto* enemy = Cast<ABaseCharacter>(GetOwner());
	ChangeState(*enemy, ECharacterState::Move);
}

void UEnemyFSMComponent::EndFSM()
{
	auto* enemy = Cast<ABaseCharacter>(GetOwner());
	ChangeState(*enemy, ECharacterState::Idle);
}
