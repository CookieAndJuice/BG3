// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Skill/Tasks/SkillTaskEndTurn.h"

void USkillTaskEndTurn::Start(UObject* WorldContext, AActor* Caster, const class USkillDefinition* Skill,
	const TArray<AActor*>& Targets)
{
	Super::Start(WorldContext, Caster, Skill, Targets);
}

void USkillTaskEndTurn::Cancel()
{
	Super::Cancel();
}
