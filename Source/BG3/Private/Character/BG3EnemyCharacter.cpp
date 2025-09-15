// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BG3EnemyCharacter.h"

#include "FSM/EnemyFSMComponent.h"


// Sets default values
ABG3EnemyCharacter::ABG3EnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FSMComp = CreateDefaultSubobject<UEnemyFSMComponent>(TEXT("FSMComp"));
}

// Called when the game starts or when spawned
void ABG3EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ABG3EnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FSMComp->UpdateBehavior();

	if (bIsMyTurn)
	{
		// 1. Get Attack Target
		// 2. Move
		// 3. Attack
		
	}
	
}

// Called to bind functionality to input
void ABG3EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABG3EnemyCharacter::SetMyTurn()
{
	bIsMyTurn = true;
	// 1. Get Attack Target
	
	// 2. Start
	FSMComp->StartFSM();
}
