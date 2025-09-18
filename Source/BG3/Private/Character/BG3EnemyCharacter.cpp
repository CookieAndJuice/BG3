// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BG3EnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "FSM/EnemyFSMComponent.h"


// Sets default values
ABG3EnemyCharacter::ABG3EnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FSMComp = CreateDefaultSubobject<UEnemyFSMComponent>(TEXT("FSMComp"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Character/Enemy/Zombie/Mesh/SKM_Zombie.SKM_Zombie'"));

	// Rotating Mesh
	FRotator Rotation = FRotator(0.f, -90.f, 0.f);
	GetMesh()->SetRelativeRotation(Rotation);

	// Locating Mesh
	FVector Location = FVector(0.000000,0.000000,-40.000000);
	GetMesh()->SetRelativeLocation(Location);

	// Resize Capsule Component
	GetCapsuleComponent()->SetCapsuleHalfHeight(42.f);
	GetCapsuleComponent()->SetCapsuleRadius(34.0);
    
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
}

// Called to bind functionality to input
void ABG3EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABG3EnemyCharacter::SetMyTurn()
{
	FSMComp->StartMyTurn();
}
