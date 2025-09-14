// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMComponent.h"
#include "EnemyFSMComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BG3_API UEnemyFSMComponent : public UFSMComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemyFSMComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void StartFSM();

	void EndFSM();
};
