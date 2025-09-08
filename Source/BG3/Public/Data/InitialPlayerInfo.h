// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InitialPlayerInfo.generated.h"

/**
 * 
 */
USTRUCT(Atomic)
struct FCharInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseCharacter> Character;
	UPROPERTY(EditAnywhere)
	FTransform SpawnTransform;
};

UCLASS()
class BG3_API UInitialPlayerInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FCharInfo> CharInfos;
};
