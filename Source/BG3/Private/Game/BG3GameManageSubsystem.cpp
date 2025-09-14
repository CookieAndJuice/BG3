// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BG3GameManageSubsystem.h"

#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Character/BG3EnemyCharacter.h"
#include "Character/BG3PlayerCharacter.h"
#include "Controller/BG3GameModePlayerController.h"
#include "Data/InitialCharacterInfo.h"
#include "Game/BG3GameMode.h"
#include "Game/BG3GameState.h"

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
			return a.TurnOrder < b.TurnOrder;
		}
		const bool bAIsPlayer = a.TurnCharacter->IsA(ABG3PlayerCharacter::StaticClass());
		const bool bBIsPlayer = b.TurnCharacter->IsA(ABG3PlayerCharacter::StaticClass());
		
		if (bAIsPlayer != bBIsPlayer)
		{
			return bAIsPlayer;
		}
		
		return false;
	});

	for (const auto& cp : CombatPawns)
	{
		PRINTLOG(TEXT("%s : %d"), *cp.TurnCharacter.GetName(), cp.TurnOrder);
	}

	// Init Round
	InitializeGameState();
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
			PRINTLOG(TEXT("Enemy %d"), order);
			FTurnData data = {order, enemy};
			CombatPawns.Add(data);
		}
	}
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
			PRINTLOG(TEXT("Player %d"), order);
			FTurnData data = {order, player};
			CombatPawns.Add(data);
		}
	}
}

void UBG3GameManageSubsystem::InitializeGameState()
{
	int playerNum = PlayerDataAsset->CharInfos.Num();
	int enemyNum = EnemyDataAsset->CharInfos.Num();
	GState->InitBG3GameState(playerNum, enemyNum);
}

void UBG3GameManageSubsystem::BeginNextTurn()
{
	// If Current Character is In Action, Cannot Begin Next Turn
	if (bIsInAction) return;
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
	// Get and Possess Next Character
	ABaseCharacter* nextCharacter = Cast<ABaseCharacter>(GetCurrentPawn());
	GMPlayerController->SwitchToPawn(nextCharacter);
}

ABaseCharacter* UBG3GameManageSubsystem::GetCurrentPawn()
{
	ABaseCharacter* pawn = CastChecked<ABaseCharacter>(CombatPawns[Index].TurnCharacter);
	return pawn;
}
