// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BG3GameCamera.h"

#include "BG3/BG3.h"
#include "Camera/CameraComponent.h"
#include "Chaos/SoftsSpring.h"
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
	MiddlePitch = (MaxPitch + MinPitch) / 2;
}

// Called when the game starts or when spawned
void ABG3GameCamera::BeginPlay()
{
	Super::BeginPlay();

	GMSubsystem = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>();
	TargetLength = SpringArmComponent->TargetArmLength;
	TargetPitch = SpringArmComponent->GetRelativeRotation().Pitch;
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
	else if (bIsAttackCameraActive)
	{
		FVector targetLoc = AttackFocusLocation;
		
		FVector NextLoc = FMath::VInterpTo(GetActorLocation(), targetLoc, DeltaTime, FocusMoveSpeed);
		SetActorLocation(NextLoc);
	}
	else
	{
		FVector targetLoc = FocusCharacter->GetActorLocation();
		
		FVector NextLoc = FMath::VInterpTo(GetActorLocation(), targetLoc, DeltaTime, FocusMoveSpeed);
		SetActorLocation(NextLoc);
	}

	UpdateZoom(DeltaTime);
}

// Called to bind functionality to input
void ABG3GameCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABG3GameCamera::FocusCamera(class ABaseCharacter* focusCharacter)
{
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

void ABG3GameCamera::AttackCamera(FVector location)
{
	AttackFocusLocation = location;
	bIsAttackCameraActive = true;
	bIsFreeCameraMode = false;
}

void ABG3GameCamera::UpdateZoom(const float DeltaTime)
{
	if (ZoomDirection != 0)
	{
		float percent = DeltaTime * ZoomAlpha;
		
		// TargetArmLength
		float curLen = SpringArmComponent->TargetArmLength;
		float nextLen = FMath::Lerp(curLen, TargetLength, percent);

		// Pitch
		float curPitch = SpringArmComponent->GetRelativeRotation().Pitch;
		float nextPitch = FMath::Lerp(curPitch, TargetPitch, percent);
		FRotator curRot = SpringArmComponent->GetRelativeRotation();
		curRot.Pitch = nextPitch;
		
		if (FMath::Abs(nextLen - curLen) < 0.05)
		{
			curRot.Pitch = TargetPitch;
			
			SpringArmComponent->TargetArmLength = TargetLength;
			SpringArmComponent->SetRelativeRotation(curRot);
			
			ZoomDirection = 0;
			PitchDirection = 0;
		}
		else
		{
			SpringArmComponent->TargetArmLength = nextLen;
			SpringArmComponent->SetRelativeRotation(curRot);
		}
	}
}

void ABG3GameCamera::Zoom(float input)
{
	// 확대면 거리가 줄도록
	ZoomDirection = -input;
	TargetLength += ZoomDirection * ZoomDistance;
	TargetLength = FMath::Clamp(TargetLength, MinTargetArmLength, MaxTargetArmLength);

	// 확대면 pitch가 커지게
	PitchDirection = input;
	TargetPitch += PitchStep * PitchDirection;
	TargetPitch = FMath::Clamp(TargetPitch, MinPitch, MaxPitch);
}

void ABG3GameCamera::RotateCamera(float input)
{
	// Rotate
	float rotatorVal = input * RotateSpeed * GetWorld()->GetDeltaSeconds();
	FRotator rotator(0.f, rotatorVal, 0.f);
	AddActorWorldRotation(rotator);
}

void ABG3GameCamera::CustomZoom(float input, float targetArmLength, float targetPitch)
{
	// 확대면 거리가 줄도록
	ZoomDirection = -input;
	TargetLength = FMath::Clamp(targetArmLength, MinTargetArmLength, MaxTargetArmLength);

	// 확대면 pitch가 커지게
	PitchDirection = input;
	TargetPitch = FMath::Clamp(targetPitch, MinPitch, MaxPitch);
}

// when Action is started
void ABG3GameCamera::PlayAttackCamera(EAttackMode attackMode, ABaseCharacter* target)
{
	// 카메라로부터 플레이어와 거리가 특정 거리보다 멀면 카메라 이동
	PreAttackMode = attackMode;
	preTargetArmLength = SpringArmComponent->TargetArmLength;
	prePitch = SpringArmComponent->GetRelativeRotation().Pitch;
	float dist = (GMSubsystem->GetCurrentPawn()->GetActorLocation() - CameraComponent->GetComponentLocation()).Size();
	PRINTLOG(TEXT("PlayAttackCamera dist : %f"), dist);
	// 근거리
	if (dist > MaxTargetArmLength + ExtraTargetArmLength)
	{
		if (PreAttackMode == EAttackMode::Melee)
		{
			// 플레이어 중심으로
			FocusCamera(GMSubsystem->GetCurrentPawn());

			// 근거리 : 중간보다 가까우면 중간으로 축소
			if (preTargetArmLength < MiddleTargetArmLength)
				CustomZoom(1, MiddleTargetArmLength, MiddlePitch);
		}
		else if (PreAttackMode == EAttackMode::Ranged)
		{
			// 적과 플레이어 중간으로
			FVector midLoc = (target->GetActorLocation() + GMSubsystem->GetCurrentPawn()->GetActorLocation()) / 2;
			AttackCamera(midLoc);
		}
	}

	// 원거리 줌
	if (PreAttackMode == EAttackMode::Ranged)
	{		
		// 그냥 축소
		CustomZoom(1, MaxTargetArmLength, MinPitch);
	}
	PRINTLOG(TEXT("PlayAttackCamera End : %d"), bIsAttackCameraActive);
}

// when Action is end
void ABG3GameCamera::StopAttackCamera()
{
	// 카메라 줌 복구
	PRINTLOG(TEXT("StopAttackCamera Start : %d"), bIsAttackCameraActive);
	if (!bIsAttackCameraActive) return;
	
	if (PreAttackMode == EAttackMode::Ranged)
	{
		FocusCamera(GMSubsystem->GetCurrentPawn());
	}
	bIsAttackCameraActive = false;
	TargetLength = preTargetArmLength;
	TargetPitch = prePitch;
	CustomZoom(1, preTargetArmLength, prePitch);
}
