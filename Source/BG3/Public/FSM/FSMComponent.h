// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BG3EnemyCharacter.h"
#include "Components/ActorComponent.h"
#include "FSMComponent.generated.h"


UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BG3_API UFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFSMComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	// Update
	virtual void UpdateBehavior() PURE_VIRTUAL(UFSMComponent::UpdateBehavior, );
};
