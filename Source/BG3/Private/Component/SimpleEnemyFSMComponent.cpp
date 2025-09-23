// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SimpleEnemyFSMComponent.h"

#include "Actor/BG3GameCamera.h"
#include "BG3/BG3.h"
#include "Character/BG3EnemyCharacter.h"
#include "Character/BG3PlayerCharacter.h"
#include "Component/CharacterStatsComponent.h"
#include "Game/BG3GameManageSubsystem.h"
#include "Game/BG3GameMode.h"
#include "Game/BG3GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Game/SkillExecutionSubsystem.h"
#include "Game/Skill/Tasks/SkillTaskPlayMontage.h"

// Sets default values for this component's properties
USimpleEnemyFSMComponent::USimpleEnemyFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void USimpleEnemyFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<ABG3EnemyCharacter>(GetOwner());
	SESubsys = GetWorld()->GetSubsystem<USkillExecutionSubsystem>();
	// endmyturn을 델리게이트 구독 코드 추가 예정
	SESubsys->OnTurnEnd.AddUObject(this, &USimpleEnemyFSMComponent::EndMyTurn);
}

void USimpleEnemyFSMComponent::ChangeState(ECharacterState state)
{
	CurrentState = state;
}

void USimpleEnemyFSMComponent::UpdateBehavior()
{
	switch (CurrentState)
	{
	case ECharacterState::Idle:
		
		IdleState();
		break;
	case ECharacterState::Plan:
		PlanState();
		break;
	case ECharacterState::Execute:
		ExecuteState();
		break;
	}
}

void USimpleEnemyFSMComponent::StartMyTurn()
{
	bIsMyTurn = true;
	
	// timer
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &USimpleEnemyFSMComponent::ChangeState, ECharacterState::Plan);
	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.7f, false);
}

void USimpleEnemyFSMComponent::EndMyTurn()
{
	bIsMyTurn = false;
	ChangeState(ECharacterState::Idle);
	GetWorld()->GetSubsystem<UBG3GameManageSubsystem>()->BeginNextTurn();
}

void USimpleEnemyFSMComponent::IdleState()
{
	PRINTSTATELOG(TEXT("Idle Idle Idle"));
}

void USimpleEnemyFSMComponent::PlanState()
{
	PRINTSTATELOG(TEXT("Plan Plan Plan"));
	
	// select target
	// 1. select ESkillKind
	// 2. melee : distance / ranged : hp
	
	TArray<AActor*> playerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABG3PlayerCharacter::StaticClass(), playerArray);

	// Set SkillID from SkillKind
	int32 typeNumber = FMath::RandRange(1, 2);
	ESkillKind skillKind;
	int32 skillID = 0;
	if (typeNumber == 1)
	{
		skillKind = ESkillKind::Melee;

		// near target
		FindNearestTarget(playerArray);
		int32 index = FMath::RandRange(0, MeleeAttackIDs.Num() - 1);
		skillID = MeleeAttackIDs[index];
	}
	else if (typeNumber == 2)
	{
		skillKind = ESkillKind::Ranged;
		PRINTLOG(TEXT("rangedddddddddd"));

		// low hp target
		FindLowestHPTarget(playerArray);
		int32 index = FMath::RandRange(0, RangedAttackIDs.Num() - 1);
		skillID = RangedAttackIDs[index];
	}
	
	DoAction(skillID);
}

void USimpleEnemyFSMComponent::ExecuteState()
{
	PRINTSTATELOG(TEXT("Execute Execute Execute"));
}

void USimpleEnemyFSMComponent::FindNearestTarget(TArray<AActor*> &targets)
{
	// Find Nearest Target
	int32 minDistance = 1e9;
	for (int32 i = 0; i < targets.Num(); i++)
	{
		ABG3PlayerCharacter* player = Cast<ABG3PlayerCharacter>(targets[i]);
		float distance = (player->GetActorLocation() - me->GetActorLocation()).Size();
		
		if (nullptr == target)
		{
			target = player;
			continue;
		}
		if (player->GetIsDead()) continue;
		
		if (distance < minDistance)
		{
			minDistance = distance;
			target = player;
		}
	}
}

void USimpleEnemyFSMComponent::FindLowestHPTarget(TArray<AActor*> &targets)
{
	// Find Lowest Hp Target
	float minHealth = 1e9;
	for (int32 i = 0; i < targets.Num(); i++)
	{
		ABG3PlayerCharacter* player = Cast<ABG3PlayerCharacter>(targets[i]);
		float health = player->Stats->GetHealth();

		if (nullptr == target)
		{
			target = player;
			continue;
		}
		if (player->GetIsDead()) continue;

		if (health < minHealth)
		{
			minHealth = health;
			target = player;
		}
	}
}

void USimpleEnemyFSMComponent::DoAction(int32 skillID)
{
	if (nullptr == target) return;
	
	float targetDistance = (target->GetActorLocation() - me->GetActorLocation()).Size();

	ABG3GameMode* gm= Cast<ABG3GameMode>(GetWorld()->GetAuthGameMode());
	
	// (이동해야 할 거리 <= 최대 이동 가능 거리) ? 액션(이동 + 스킬) 발동 : 액션(이동만) 발동
	if (targetDistance <= me->Stats->GetRemainingMoveDistance())
	{
		// move & attack
		bool reqSuccess = gm->RequestUseSkill(me, skillID);
		
		if (reqSuccess)
		{
			TArray<AActor*> tempTargetArray;
			tempTargetArray.Add(target);
			SESubsys->SetTargets(tempTargetArray);
			
			SESubsys->ConfirmAndExecute(Cast<ABG3GameState>(GetWorld()->GetGameState())->GetCurrentRound());
		}
	}
	else
	{
		// move only
		skillID = MoveActionID;
		bool reqSuccess = gm->RequestUseSkill(me, skillID);
		
		if (reqSuccess)
		{
			TArray<AActor*> tempTargetArray;
			tempTargetArray.Add(target);
			SESubsys->SetTargets(tempTargetArray);
			
			SESubsys->ConfirmAndExecute(Cast<ABG3GameState>(GetWorld()->GetGameState())->GetCurrentRound());
		}
	}

	// change state to ExecuteState
	ChangeState(ECharacterState::Execute);
}

void USimpleEnemyFSMComponent::BindingMontageTask(USkillTaskPlayMontage* InTask)
{
	if (!InTask) return;
	MontageTask = InTask;

	MontageTask->OnAnimEnded.AddUObject(this, &USimpleEnemyFSMComponent::EndMyTurn);
}
