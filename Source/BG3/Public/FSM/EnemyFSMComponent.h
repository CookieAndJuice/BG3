// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyFSMComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	None, Idle, Plan, Move, Execute,
	Hit, Die
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
	ECharacterState CurrentState = ECharacterState::None;
	
public: // Change State & Update Behavior    
	virtual void ChangeState(ECharacterState state);

	virtual void UpdateBehavior();
	
	void StartMyTurn();

	void EndMyTurn();

public:	// FSM
	void IdleState();
	void PlanState();
	void MoveState();
	void ExecuteState();
	void HitState();
	void DieState();

protected:
	bool bIsMyTurn = false;
};
