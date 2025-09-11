
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SkillExecutionSubsystem.generated.h"

class ABG3GameMode;
class USkillDefinition;
class ABaseCharacter;

enum class ECastState : uint8
{
	Idle,
	Targeting,
	Resolving,
	Applying,
	Completed,
	Canceled
};

struct FSkillResult
{
	bool bSuccess;
	TArray<AActor*> Affected;
	int32 TotalDamage;
};

DECLARE_DELEGATE_TwoParams(FOnCastingStarted, ABaseCharacter* /*Caster*/, const USkillDefinition* /*Skill*/);
DECLARE_DELEGATE_OneParam(FOnTargetsUpdated, const TArray<AActor*>& /*Targets*/);
DECLARE_DELEGATE_OneParam(FOnSkillResolved, const FSkillResult& /*Result*/);
DECLARE_DELEGATE(FOnCastingCanceled);
DECLARE_DELEGATE_OneParam(FOnCastingFailed, FName /*Fail*/);

UCLASS()
class BG3_API USkillExecutionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	bool RequestCast(ABaseCharacter* Caster, USkillDefinition* Skill);
	void CancelCast();
	void SetTargets(const TArray<AActor*>& InTargets);
	bool ConfirmAndExecute(int32 CurrentRound);
	bool IsBusy() const;
	ECastState GetCastState() const;

	FOnCastingStarted CastingStarted;
	FOnCastingFailed CastFailed;

private:
	ECastState CastState;
	FSkillResult CurrentSkillResult;
	TWeakObjectPtr<ABG3GameMode> GameMode;
	TWeakObjectPtr<UBG3GameManageSubsystem> TurnSubsystem;
};
