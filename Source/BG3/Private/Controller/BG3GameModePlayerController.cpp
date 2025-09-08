// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BG3GameModePlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "BG3/Public/Actor/BG3GameCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/BaseCharacter.h"
#include "Game/BG3GameManageSubsystem.h"

ABG3GameModePlayerController::ABG3GameModePlayerController()
{
	
}

void ABG3GameModePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// camera setting
	SpawnCamera();
	SetViewTargetWithBlend(BG3Camera);
}

void ABG3GameModePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABG3GameModePlayerController::SwitchToPawn(ABaseCharacter* NewCharacter)
{
	if (!NewCharacter) return;

	// possess new character
	auto* GMSubsys = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>();
	GMSubsys->GetCurrentPawn()->DisableInput(this);
	
	Possess(NewCharacter);
	NewCharacter->EnableInput(this);

	// switch camera target
	// change UI skill info
}

void ABG3GameModePlayerController::SpawnCamera()
{
	FRotator spawnRotation = FRotator(0, 0, 0);
	FVector spawnLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();

	FTransform spawnTransform(spawnRotation, spawnLocation);

	// 이것보다는 폰에 부착하도록 만들어보기
	BG3Camera = GetWorld()->SpawnActor<ABG3GameCamera>(BG3CameraClass, spawnTransform);
	BG3Camera->SpringArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
}

void ABG3GameModePlayerController::SetCameraLocation(FVector location)
{
	BG3Camera->SetActorLocation(location);
}
