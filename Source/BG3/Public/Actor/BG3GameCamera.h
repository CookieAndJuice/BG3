// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BG3GameCamera.generated.h"

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
	
	FVector PreDirection;
	float ZoomDirection;
	float ZoomTarget;
	bool bDoesCameraMove = false;

public:	// Camera Movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float FreeDirMoveSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float FreeLocMoveSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float FocusMoveSpeed = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float RotateSpeed = 75.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float ZoomDistance = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float ZoomSpeed = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MaxTargetArmLength = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MinTargetArmLength = 400.f;

	float MiddleTargetArmLength = 0.f;
	
public:	// Camera Movement
	void FocusCamera(class ABaseCharacter* focusCharacter);
	
	void FreeCamera(FVector2D direction);
	
	void Zoom(float input);

	void RotateCamera(float input);

	void CustomZoom(float input, float targetArmLength);
};
