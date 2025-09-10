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

public: // Camerae State
	void SetFreeCameraMode(bool val);
	
private:// Camera State
	bool bIsFreeCameraMode = false;

private:// Camera Movement
	float Dx;
	float Dy;
	FVector PreDirection;
	bool bDoesCameraMove = false;

public:	// Camera Movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MoveSpeed = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float RotateSpeed = 75.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float ZoomSpeed = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MaxTargetArmLength = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MinTargetArmLength = 300.f;
	
public:	// Camera Movement
	void FocusCamera(FVector location);
	
	void FreeCamera(FVector2D direction);
	
	void Zoom(float input);

	void RotateCamera(float input);
};
