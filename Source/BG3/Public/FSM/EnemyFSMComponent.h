// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyFSMComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	None, Idle, Plan, Execute
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BG3_API UEnemyFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemyFSMComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Current Enum State
	UPROPERTY(VisibleInstanceOnly, Category = "FSM")
	ECharacterState CurrentState = ECharacterState::Idle;
	
public: // Change State & Update Behavior    
	virtual void ChangeState(ECharacterState state);

	virtual void UpdateBehavior();
	
	void StartMyTurn();

	void EndMyTurn();

public:	// FSM
	void IdleState();
	void PlanState();
	void ExecuteState();

protected:
	bool bIsMyTurn = false;

	// Max Move Distance
	float MaxDistance = 0;

	UPROPERTY()
	TObjectPtr<class ABG3PlayerCharacter> target = nullptr;

	UPROPERTY()
	TObjectPtr<class ABG3EnemyCharacter> me;
};
