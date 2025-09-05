// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/BG3CameraManager.h"

// Sets default values
ABG3CameraManager::ABG3CameraManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABG3CameraManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABG3CameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


