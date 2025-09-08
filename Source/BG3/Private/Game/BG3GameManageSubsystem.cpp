// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BG3GameManageSubsystem.h"

#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Character/BG3PlayerCharacter.h"
#include "Data/InitialPlayerInfo.h"

UBG3GameManageSubsystem::UBG3GameManageSubsystem()
{
	ConstructorHelpers::FObjectFinder<UInitialPlayerInfo> playerInfosRef(TEXT("/Script/BG3.InitialPlayerInfo'/Game/Blueprints/Data/DA_InitialCharInfo.DA_InitialCharInfo'"));
	if (playerInfosRef.Succeeded())
	{
		PlayerDataAsset = playerInfosRef.Object;
	}
}

void UBG3GameManageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SpawnPlayers();
	SpawnEnemies();
	SortCombatPawns();
}

void UBG3GameManageSubsystem::SpawnEnemies()
{
	// spawn & init
}

void UBG3GameManageSubsystem::SpawnPlayers()
{
	// spawn & init
	for (const auto& playerInfo : PlayerDataAsset->CharInfos)
	{
		auto player = GetWorld()->SpawnActor<ABG3PlayerCharacter>(playerInfo.Character, playerInfo.SpawnTransform);
		if (player)
		{
			PRINTLOG(TEXT("player"));
			FTurnData data = {0, player};
			CombatPawns.Add(data);
		}
	}
}

void UBG3GameManageSubsystem::SortCombatPawns()
{
	// sort 기준 함수를 별도 구현해야 할 듯
	
	// 선제권 값이 같을 경우
	// 다를 경우
}

void UBG3GameManageSubsystem::InitializeCamera()
{
	// 카메라 스폰
	// 폰에 카메라 붙이기 (attach actor to component)
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
