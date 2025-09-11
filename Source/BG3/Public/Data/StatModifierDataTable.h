// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StatModifierDataTable.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FStatModifierData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_2;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_3;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_4;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_5;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_6;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_7;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_9;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_11;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_12;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_13;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_14;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_15;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_16;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_17;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_18;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_19;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_20;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_21;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_22;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_23;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_24;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_25;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_26;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_27;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_28;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_29;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatModifier")
    int32 Score_30;
};

class BG3_API StatModifierDataTable
{
public:
	StatModifierDataTable();
	~StatModifierDataTable();
};
