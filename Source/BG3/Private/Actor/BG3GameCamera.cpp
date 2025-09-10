// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BG3GameCamera.h"

#include "BG3/BG3.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABG3GameCamera::ABG3GameCamera()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BG3RootComponent = CreateDefaultSubobject<USceneComponent>("BG3RootComponent");
	SetRootComponent(BG3RootComponent);

	CameraBaseComponent = CreateDefaultSubobject<USceneComponent>("CameraBaseComponent");
	CameraBaseComponent->SetupAttachment(GetRootComponent());
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(CameraBaseComponent);
	SpringArmComponent->TargetArmLength = 1000.f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

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

	if (bIsFreeCameraMode)
	{
		PRINTLOG(TEXT("FreeEEEEEEEEEEE!!!"));
		
		FVector desiredDir(GetActorForwardVector() * Dx + GetActorRightVector() * Dy);
		desiredDir = FMath::VInterpTo(PreDirection, desiredDir, DeltaTime, MoveSpeed);
		FVector target(GetActorLocation() + desiredDir * MoveSpeed * DeltaTime);
		SetActorLocation(target);

		Dx = 0; Dy = 0;
	}
	else
	{
		PRINTLOG(TEXT("dddddddddd"));
		SetActorLocation(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation());
	}
}

// Called to bind functionality to input
void ABG3GameCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABG3GameCamera::SetFreeCameraMode(bool val)
{
	bIsFreeCameraMode = val;
}

void ABG3GameCamera::FocusCamera(FVector location)
{
	bIsFreeCameraMode = false;
	SetActorLocation(location);
}

void ABG3GameCamera::FreeCamera(FVector2D direction)
{
	bIsFreeCameraMode = true;
	direction.Normalize();
	Dx = direction.X;
	Dy = direction.Y;	
}

void ABG3GameCamera::Zoom(float input)
{
	// Scaling
}

void ABG3GameCamera::RotateCamera(float input)
{
	// Rotate
	float rotatorVal = input * RotateSpeed * GetWorld()->GetDeltaSeconds();
	FRotator rotator(0.f, rotatorVal, 0.f);
	AddActorWorldRotation(rotator);
}
