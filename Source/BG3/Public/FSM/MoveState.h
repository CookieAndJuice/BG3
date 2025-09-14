// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMComponent.h"
#include "MoveState.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BG3_API UMoveState : public UFSMComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMoveState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ChangeState(ABaseCharacter& character, ECharacterState state) override;

	virtual void UpdateBehavior() override;
};
