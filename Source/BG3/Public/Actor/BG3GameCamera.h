// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BG3GameCamera.generated.h"

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

private: // root component
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USceneComponent> BG3RootComponent;
	
public:	// camera
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<class UCameraComponent> CameraComponent;
};
