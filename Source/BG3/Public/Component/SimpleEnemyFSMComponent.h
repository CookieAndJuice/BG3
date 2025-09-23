// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleEnemyFSMComponent.generated.h"

class USkillTaskPlayMontage;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	None, Idle, Plan, Execute
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BG3_API USimpleEnemyFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimpleEnemyFSMComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Current Enum State
	UPROPERTY(VisibleInstanceOnly, Category = "FSM")
	ECharacterState CurrentState = ECharacterState::Idle;

public: // Change State & Update Behavior
	UFUNCTION()
	virtual void ChangeState(ECharacterState state);

	virtual void UpdateBehavior();
	
	void StartMyTurn();

	void EndMyTurn();
	
public:	// FSM
	void IdleState();
	void PlanState();
	void ExecuteState();

private:// Helper Function
	void FindNearestTarget(TArray<AActor*> &targets);
	void FindLowestHPTarget();
	void DoAction(int32 skillID);

public: // Montage Task
	void BindingMontageTask(USkillTaskPlayMontage* InTask);
	
	UPROPERTY()
	USkillTaskPlayMontage* MontageTask;

protected:
	UPROPERTY()
	TObjectPtr<class USkillExecutionSubsystem> SESubsys;
	
	bool bIsMyTurn = false;
	
	UPROPERTY()
	TObjectPtr<class ABG3PlayerCharacter> target = nullptr;

	UPROPERTY()
	TObjectPtr<class ABG3EnemyCharacter> me;

	UPROPERTY()
	TArray<int32> MeleeAttackIDs = {1, 101};

	UPROPERTY()
	TArray<int32> RangedAttackIDs = {20};

	UPROPERTY()
	int32 MoveActionID = 888;
};
