// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InitialCharacterInfo.generated.h"

/**
 * 
 */
USTRUCT(Atomic)
struct FCharInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 CharacterID;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseCharacter> Character;
	UPROPERTY(EditAnywhere)
	FTransform SpawnTransform;
	UPROPERTY(EditAnywhere)
	class UTexture2D* Portrait;
	UPROPERTY(EditAnywhere)
	class UTexture2D* SmallPortrait;
};

UCLASS()
class BG3_API UInitialCharacterInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FCharInfo> CharInfos;
};
