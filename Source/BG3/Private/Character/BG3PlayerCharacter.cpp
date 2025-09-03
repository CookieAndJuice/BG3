// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BG3PlayerCharacter.h"


// Sets default values
ABG3PlayerCharacter::ABG3PlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABG3PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABG3PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABG3PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

