// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BG3GameCamera.generated.h"

UENUM()
enum class EAttackMode : uint8
{
	Melee, Ranged
};

UCLASS()
class BG3_API ABG3GameCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABG3GameCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY()
	TObjectPtr<class UBG3GameManageSubsystem> GMSubsystem;
	
private: // Root Component
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USceneComponent> BG3RootComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USceneComponent> CameraBaseComponent;
	
public:	// Camera
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<class UCameraComponent> CameraComponent;
	
private:// Camera State
	bool bIsFreeCameraMode = false;

private:// Camera Movement
	float Dx;
	float Dy;

	UPROPERTY()
	TObjectPtr<class ABaseCharacter> FocusCharacter;
	
	FVector PreDirection = FVector::ZeroVector;
	float ZoomDirection = 0;
	float ZoomTarget = 0;
	bool bDoesCameraMove = false;

	float TargetPitch = 0;
	float PitchDirection = 0;
	float ZoomAlpha = 0;

public:	// Camera Movement
	// Camera Move
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraActorMove")
	float FreeDirMoveSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraActorMove")
	float FreeLocMoveSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraActorMove")
	float FocusMoveSpeed = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraActorMove")
	float RotateSpeed = 90.f;

	// Camera Zoom
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraZoom")
	float ZoomDistance = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraZoom")
	float ZoomSpeed = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraZoom")
	float MaxTargetArmLength = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraZoom")
	float MinTargetArmLength = 600.f;

	float MiddleTargetArmLength = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraZoom")
	float MaxPitch = -20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraZoom")
	float MinPitch = -60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraZoom")
	float PitchStep = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraZoom")
	float PitchInterpSpeed = 2.5f;

private:// Camera Movement for Record during Action
	float preTargetArmLength = 0.f;
	
public:	// Camera Movement
	void FocusCamera(class ABaseCharacter* focusCharacter);
	
	void FreeCamera(FVector2D direction);
	
	void Zoom(float input);

	void RotateCamera(float input);

	void CustomZoom(float input, float targetArmLength);
	
	void PlayAttackCamera(EAttackMode attackMode, class ABaseCharacter* target);

	void StopAttackCamera();
};
