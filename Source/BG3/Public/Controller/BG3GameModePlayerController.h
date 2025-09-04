// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BG3GameModePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BG3_API ABG3GameModePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABG3GameModePlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
public:	// camera
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class ABG3GameCamera> BG3CameraClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class ABG3GameCamera> BG3Camera;
	
	void SpawnCamera();

	UFUNCTION(BlueprintCallable)
	void SetCameraLocation(FVector location);
};
