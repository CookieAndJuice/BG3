// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BG3GameCamera.h"

#include "BG3/BG3.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/BG3GameManageSubsystem.h"
#include "Character/BaseCharacter.h"


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
	SpringArmComponent->TargetArmLength = MaxTargetArmLength;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	MiddleTargetArmLength = (MaxTargetArmLength + MinTargetArmLength) / 2;
}

// Called when the game starts or when spawned
void ABG3GameCamera::BeginPlay()
{
	Super::BeginPlay();

	GMSubsystem = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>();
	ZoomTarget = SpringArmComponent->TargetArmLength;
}

// Called every frame
void ABG3GameCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFreeCameraMode)
	{
		FVector desiredDir(GetActorForwardVector() * Dx + GetActorRightVector() * Dy);
		PreDirection = FMath::VInterpTo(PreDirection, desiredDir, DeltaTime, FreeDirMoveSpeed);
		
		FVector target(GetActorLocation() + PreDirection * FreeLocMoveSpeed * DeltaTime);
		SetActorLocation(target);

		Dx = 0; Dy = 0;
	}
	else
	{
		FVector targetLoc = FocusCharacter->GetActorLocation();
		
		FVector NextLoc = FMath::VInterpTo(GetActorLocation(), targetLoc, DeltaTime, FocusMoveSpeed);
		SetActorLocation(NextLoc);
	}

	if (ZoomDirection != 0)
	{
		float cur = SpringArmComponent->TargetArmLength;
		
		float length = FMath::FInterpTo(cur, ZoomTarget, DeltaTime, ZoomSpeed);
		SpringArmComponent->TargetArmLength = length;
		
		if (FMath::Abs(ZoomTarget - cur) < 0.05)
		{
			cur = ZoomTarget;
			ZoomDirection = 0;
		}
	}
}

// Called to bind functionality to input
void ABG3GameCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABG3GameCamera::FocusCamera(class ABaseCharacter* focusCharacter)
{
	// location 넣는 게 맞을 지도
	FocusCharacter = focusCharacter;
	bIsFreeCameraMode = false;
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
	ZoomDirection = -input;
	ZoomTarget += ZoomDirection * ZoomDistance;
	ZoomTarget = FMath::Clamp(ZoomTarget, MinTargetArmLength, MaxTargetArmLength);
}

void ABG3GameCamera::RotateCamera(float input)
{
	// Rotate
	float rotatorVal = input * RotateSpeed * GetWorld()->GetDeltaSeconds();
	FRotator rotator(0.f, rotatorVal, 0.f);
	AddActorWorldRotation(rotator);
}

void ABG3GameCamera::CustomZoom(float input, float targetArmLength)
{
	ZoomDirection = -input;
	ZoomTarget = FMath::Clamp(targetArmLength, MinTargetArmLength, MaxTargetArmLength);
}
