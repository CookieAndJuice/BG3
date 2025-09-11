
#include "Game/SkillExecutionSubsystem.h"
#include "Character/BaseCharacter.h"
#include "Component/SkillBookComponent.h"

bool USkillExecutionSubsystem::RequestCast(ABaseCharacter* Caster, USkillDefinition* Skill)
{
	if (CastState != ECastState::Idle)
	{
		CastFailed.Execute("Busy");
		return false;
	}
	
	if (!Caster || !Skill || !Caster->SkillBook->Skills.Contains(Skill) )
	{
		CastFailed.Execute("No Caster or Skill");
		return false;
	}
	
	bool OutResult;
	Caster->SkillBook->GetUsability(Skill, OutResult);
	if (!OutResult) return false;

	if (Caster->SkillBook->ReserveUse(Skill))
	{
		
		CastState = ECastState::Targeting;
		CastingStarted.Execute(Caster, Skill);
		return true;
	}
	else
	{
		CastFailed.Execute("Reserve Failed");
		return false;
	}
}

void USkillExecutionSubsystem::CancelCast()
{
}

void USkillExecutionSubsystem::SetTargets(const TArray<AActor*>& InTargets)
{
}

bool USkillExecutionSubsystem::ConfirmAndExecute(int32 CurrentRound)
{
}

bool USkillExecutionSubsystem::IsBusy() const
{
	
}

ECastState USkillExecutionSubsystem::GetCastState() const
{
}
