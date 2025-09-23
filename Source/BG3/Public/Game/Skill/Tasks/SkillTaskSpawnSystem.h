// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/Tasks/SkillTaskBase.h"
#include "SkillTaskSpawnSystem.generated.h"

/**
 * 
 */
UCLASS()
class BG3_API USkillTaskSpawnSystem : public USkillTaskBase
{
	GENERATED_BODY()

public:
	virtual void Start(UObject* WorldContext, AActor* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets) override;
	virtual void Cancel() override;

	
};
