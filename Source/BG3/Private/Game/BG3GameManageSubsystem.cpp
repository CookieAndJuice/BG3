// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BG3GameManageSubsystem.h"

#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Character/BG3EnemyCharacter.h"
#include "Character/BG3PlayerCharacter.h"
#include "Controller/BG3GameModePlayerController.h"
#include "Data/InitialCharacterInfo.h"
#include "Component/SimpleEnemyFSMComponent.h"
#include "Game/BG3GameMode.h"
#include "Game/BG3GameState.h"
#include "Game/SkillExecutionSubsystem.h"
#include "Component/TurnWidgetManagerComponent.h"
#include "UI/Widget/TurnOrderFrameWidget.h"

UBG3GameManageSubsystem::UBG3GameManageSubsystem()
{
	ConstructorHelpers::FObjectFinder<UInitialCharacterInfo> playerInfosRef(TEXT("/Script/BG3.InitialCharacterInfo'/Game/Blueprints/Data/DA_InitPlayerInfo.DA_InitPlayerInfo'"));
	if (playerInfosRef.Succeeded())
	{
		PlayerDataAsset = playerInfosRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInitialCharacterInfo> enemyInfosRef(TEXT("/Script/BG3.InitialCharacterInfo'/Game/Blueprints/Data/DA_InitEnemyInfo.DA_InitEnemyInfo'"));
	if (enemyInfosRef.Succeeded())
	{
		EnemyDataAsset = enemyInfosRef.Object;
	}
}

bool UBG3GameManageSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	if (!World) return false;

	// 월드 이름으로 필터링
	return Super::ShouldCreateSubsystem(Outer) && (World->GetFName() == FName(TEXT("Dream_Island")));
}

void UBG3GameManageSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	GM = Cast<ABG3GameMode>(InWorld.GetAuthGameMode());
	GState = Cast<ABG3GameState>(InWorld.GetGameState());
	GMPlayerController = Cast<ABG3GameModePlayerController>(InWorld.GetFirstPlayerController());
	
	// Init Pawns
	SpawnPlayers();
	SpawnEnemies();
	CombatPawns.StableSort([](FTurnData a, FTurnData b)->bool
	{
		if (a.TurnOrder != b.TurnOrder)
		{
			return a.TurnOrder > b.TurnOrder;
		}
		const bool bAIsPlayer = a.TurnCharacter->IsA(ABG3PlayerCharacter::StaticClass());
		const bool bBIsPlayer = b.TurnCharacter->IsA(ABG3PlayerCharacter::StaticClass());
		
		if (bAIsPlayer != bBIsPlayer)
		{
			return bBIsPlayer;
		}
		
		return false;
	});

	for (const auto& cp : CombatPawns)
	{
		PRINTLOG(TEXT("%s : %d"), *cp.TurnCharacter.GetName(), cp.TurnOrder);
	}

	// Init Round
	InitializeGame();
	GState->PrintCurrentState();
}

void UBG3GameManageSubsystem::SpawnPlayers()
{
	// Spawn & Init
	for (const auto& playerInfo : PlayerDataAsset->CharInfos)
	{
		auto player = GetWorld()->SpawnActor<ABG3PlayerCharacter>(playerInfo.Character, playerInfo.SpawnTransform);
		if (player)
		{
			int32 order = GM->CalcInitiative(10);
			player->SetID(playerInfo.CharacterID);
			player->SetPortrait(playerInfo.Portrait);
			player->SetSmallPortrait(playerInfo.SmallPortrait);
			PlayerMap.Add(playerInfo.CharacterID, player);
			
			FTurnData data = {order, player};
			//player->SpawnDefaultController();
			CombatPawns.Add(data);
		}
	}
}

void UBG3GameManageSubsystem::SpawnEnemies()
{
	// Spawn & Init
	for (const auto& enemyInfo : EnemyDataAsset->CharInfos)
	{
		auto enemy = GetWorld()->SpawnActor<ABG3EnemyCharacter>(enemyInfo.Character, enemyInfo.SpawnTransform);
		if (enemy)
		{
			int32 order = GM->CalcInitiative(5);
			enemy->SetID(enemyInfo.CharacterID);
			enemy->SetPortrait(enemyInfo.Portrait);
			enemy->SetSmallPortrait(enemyInfo.SmallPortrait);
			EnemyMap.Add(enemyInfo.CharacterID, enemy);
			
			FTurnData data = {order, enemy};
			//enemy->SpawnDefaultController();
			CombatPawns.Add(data);
		}
	}
}

void UBG3GameManageSubsystem::InitializeGame()
{
	int playerNum = PlayerDataAsset->CharInfos.Num();
	int enemyNum = EnemyDataAsset->CharInfos.Num();
	GState->InitBG3GameState(playerNum, enemyNum);

	if (auto* enemy = Cast<ABG3EnemyCharacter>(GetCurrentPawn()))
	{
		enemy->SetMyTurn();
	}
}

void UBG3GameManageSubsystem::BeginNextTurn()
{
	// If Current Character is In Action, Cannot Begin Next Turn
	if (bIsInAction) return;

	// Defensive: ensure no lingering cast carries into next turn
	if (UWorld* World = GetWorld())
	{
		if (USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
		{
			if (SES->IsBusy())
			{
				PRINTLOG(TEXT("[BeginNextTurn] SES busy; cancel cast before advancing turn"));
				SES->CancelCast();
			}
		}
	}
	PRINTLOG(TEXT("Next Turn!!!!"));
	
	// Next Turn or Round
	if (CombatPawns.Num() > ++Index)
	{
		GState->UpdateTurn(); 
	}
	else
	{
		Index = 0;
		GState->UpdateRound();
	}
	GState->PrintCurrentState();
	
	// Get and Possess Next Character
	ABaseCharacter* nextCharacter = GetCurrentPawn();
	GMPlayerController->SwitchToPawn(nextCharacter);
	
}

ABaseCharacter* UBG3GameManageSubsystem::GetCurrentPawn()
{
	ABaseCharacter* pawn = CastChecked<ABaseCharacter>(CombatPawns[Index].TurnCharacter);
	return pawn;
}

ABG3PlayerCharacter* UBG3GameManageSubsystem::GetPlayerFromID(int32 id)
{
	return PlayerMap.FindRef(id);
}

ABG3EnemyCharacter* UBG3GameManageSubsystem::GetEnemyFromID(int32 id)
{
	return EnemyMap.FindRef(id);
}

void UBG3GameManageSubsystem::RemoveCharacterFromCombatPawns(ABaseCharacter* InCharacter)
{
	int removeIndex = GetCombatPawnsIndex(InCharacter);
	if (removeIndex != -1)
	{
		if (removeIndex < Index)
			Index--;
		CombatPawns.RemoveAt(removeIndex);
	}
}

int UBG3GameManageSubsystem::GetCombatPawnsIndex(ABaseCharacter* InCharacter)
{
	for (int i=0; i<CombatPawns.Num(); i++)
	{
		if (CombatPawns[i].TurnCharacter == InCharacter)
		{
			return i;
		}
	}

	return -1;
}
