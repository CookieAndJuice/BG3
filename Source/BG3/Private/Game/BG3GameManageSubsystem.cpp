// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BG3GameManageSubsystem.h"

#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Character/BG3PlayerCharacter.h"
#include "Data/InitialPlayerInfo.h"
#include "Game/BG3GameMode.h"

UBG3GameManageSubsystem::UBG3GameManageSubsystem()
{
	ConstructorHelpers::FObjectFinder<UInitialPlayerInfo> playerInfosRef(TEXT("/Script/BG3.InitialPlayerInfo'/Game/Blueprints/Data/DA_InitialCharInfo.DA_InitialCharInfo'"));
	if (playerInfosRef.Succeeded())
	{
		PlayerDataAsset = playerInfosRef.Object;
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
}

void UBG3GameManageSubsystem::SpawnPlayers()
{
	// Spawn & Init
	for (const auto& playerInfo : PlayerDataAsset->CharInfos)
	{
		auto player = GetWorld()->SpawnActor<ABG3PlayerCharacter>(playerInfo.Character, playerInfo.SpawnTransform);
		if (player)
		{
			int32 order = GM->CalcInitiative(1);
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
