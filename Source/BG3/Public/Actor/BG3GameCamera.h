// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BG3GameCamera.generated.h"

UENUM()
enum class EGameCameraViewMode : uint8
{
	FocusMode,
	FreeMode
};

UCLASS()
class BG3_API ABG3GameCamera : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABG3GameCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private: // Root Component
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USceneComponent> BG3RootComponent;
	
public:	// Camera
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<class UCameraComponent> CameraComponent;

private:// Camera State
	EGameCameraViewMode ViewMode = EGameCameraViewMode::FocusMode;

public:	// Camera State
	__declspec(property(get=GetViewMode)) EGameCameraViewMode VIEWMODE;

	EGameCameraViewMode GetViewMode();
	
	void ChangeViewMode(EGameCameraViewMode NewViewMode);

private:// Camera Movement
	int8 bDoesCameraMove : 1 = false;
	
public:	// Camera Movement
	void FocusCamera(FVector location);
	
	void FreeCamera(FVector2D direction);
	
	void Zoom(float input);
};
