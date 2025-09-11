// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BG3GameManageSubsystem.h"

#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Character/BG3EnemyCharacter.h"
#include "Character/BG3PlayerCharacter.h"
#include "Data/InitialCharacterInfo.h"
#include "Game/BG3GameMode.h"

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

	GM = Cast<ABG3GameMode>(GetWorld()->GetAuthGameMode());
	
	// Init Pawns
	SpawnPlayers();
	SpawnEnemies();
	SortCombatPawns();
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

void UBG3GameManageSubsystem::SortCombatPawns()
{
	// Sort 기준 함수를 별도 구현해야 할 듯
	
	// 선제권 값이 같을 경우
	// 다를 경우
}

void UBG3GameManageSubsystem::BeginNextTurn()
{
	
}

ABaseCharacter* UBG3GameManageSubsystem::GetCurrentPawn()
{
	ABaseCharacter* pawn = CastChecked<ABaseCharacter>(CombatPawns[Index].TurnCharacter);
	Index += Index % CombatPawns.Num();
	return pawn;
}
