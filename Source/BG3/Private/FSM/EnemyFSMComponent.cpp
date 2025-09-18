
#include "FSM/EnemyFSMComponent.h"

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


UEnemyFSMComponent::UEnemyFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UEnemyFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<ABG3EnemyCharacter>(GetOwner());
	SESubsys = GetWorld()->GetSubsystem<USkillExecutionSubsystem>();
	// endmyturn을 델리게이트 구독 코드 추가 예정
	SESubsys->OnTurnEnd.AddUObject(this, &UEnemyFSMComponent::EndMyTurn);
}

void UEnemyFSMComponent::ChangeState(ECharacterState state)
{
	CurrentState = state;
}

void UEnemyFSMComponent::UpdateBehavior()
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

void UEnemyFSMComponent::StartMyTurn()
{
	bIsMyTurn = true;
	
	// timer
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &UEnemyFSMComponent::ChangeState, ECharacterState::Plan);
	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.7f, false);
}

void UEnemyFSMComponent::EndMyTurn()
{
	bIsMyTurn = false;
	ChangeState(ECharacterState::Idle);
	GetWorld()->GetSubsystem<UBG3GameManageSubsystem>()->BeginNextTurn();
}

void UEnemyFSMComponent::IdleState()
{
	PRINTSTATELOG(TEXT("Idle Idle Idle"));
}

void UEnemyFSMComponent::PlanState()
{
	PRINTSTATELOG(TEXT("Plan Plan Plan"));
	
	// select target

	// 1. max distance
	// if same 2. hp

	TArray<AActor*> playerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABG3PlayerCharacter::StaticClass(), playerArray);
	for (int32 i = 0; i < playerArray.Num(); i++)
	{
		ABG3PlayerCharacter* player = Cast<ABG3PlayerCharacter>(playerArray[i]);
		FVector dir = player->GetActorLocation() - me->GetActorLocation();
		float distance = dir.Size();
		
		if (nullptr == target)
		{
			target = player;
			continue;
		}
		
		if (distance > MaxDistance)
			continue;

		if (Cast<ABG3PlayerCharacter>(player)->Stats->Health < Cast<ABG3PlayerCharacter>(target)->Stats->Health)
		{
			target = player;
		}
	}
	
	// target distance
	float targetDistance = 0;
	if (nullptr != target)
	{
		targetDistance = (target->GetActorLocation() - me->GetActorLocation()).Size();
		PRINTSTATELOG(TEXT("%f"), targetDistance);

		ABG3GameMode* gm= Cast<ABG3GameMode>(GetWorld()->GetAuthGameMode());
		
		// select action
		//if (targetDistance <= MaxDistance)
		{
			// melee
			bool reqSuccess = gm->RequestUseSkill(me, 1);
			if (reqSuccess)
			{
				TArray<AActor*> tempTargetArray;
				tempTargetArray.Add(target);
				SESubsys->SetTargets(tempTargetArray);
				
				SESubsys->ConfirmAndExecute(Cast<ABG3GameState>(GetWorld()->GetGameState())->GetCurrentRound());
			}
		}
		{
			// ranged
		}

		// change state to ExecuteState
		ChangeState(ECharacterState::Execute);
	}
	// if cannot do anything (cause of slots) -> EndMyTurn()
	
}

void UEnemyFSMComponent::ExecuteState()
{
	PRINTSTATELOG(TEXT("Execute Execute Execute"));
}

void UEnemyFSMComponent::BindingMontageTask(USkillTaskPlayMontage* InTask)
{
	if (!InTask) return;
	MontageTask = InTask;

	MontageTask->OnAnimEnded.AddUObject(this, &UEnemyFSMComponent::EndMyTurn);
}
