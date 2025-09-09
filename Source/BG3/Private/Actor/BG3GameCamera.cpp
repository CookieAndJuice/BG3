// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BG3GameCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABG3GameCamera::ABG3GameCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BG3RootComponent = CreateDefaultSubobject<USceneComponent>("BG3RootComponent");
	SetRootComponent(BG3RootComponent);
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 1000.f;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ABG3GameCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABG3GameCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ViewMode == EGameCameraViewMode::FocusMode)
	{
		SetActorLocation(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation());
	}
}

EGameCameraViewMode ABG3GameCamera::GetViewMode()
{
	return ViewMode;
}

void ABG3GameCamera::ChangeViewMode(EGameCameraViewMode NewViewMode)
{
	ViewMode = NewViewMode;
}

void ABG3GameCamera::FocusCamera(FVector location)
{
	ChangeViewMode(EGameCameraViewMode::FocusMode);
	SetActorLocation(location);
}

void ABG3GameCamera::FreeCamera(FVector2D direction)
{
	ChangeViewMode(EGameCameraViewMode::FreeMode);
	// 카메라 actor -> pawn으로 바꿔야 함...
}

void ABG3GameCamera::Zoom(float input)
{
	// Scaling
}
