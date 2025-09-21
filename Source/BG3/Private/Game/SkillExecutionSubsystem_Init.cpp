#include "Game/SkillExecutionSubsystem.h"
#include "Game/Skill/SkillCastExecutor.h"

void USkillExecutionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (!SkillExecutor)
    {
        SkillExecutor = NewObject<USkillCastExecutor>(this);
    }

    if (SkillExecutor)
    {
        CastingStarted.RemoveAll(this);
        CastingStarted.AddUObject(SkillExecutor, &USkillCastExecutor::OnCastingStarted);
        CastConfirmed.Unbind();
        CastConfirmed.BindUObject(SkillExecutor, &USkillCastExecutor::OnCastConfirmed);
    }
}

void USkillExecutionSubsystem::Deinitialize()
{
    CastingStarted.RemoveAll(this);
    SkillExecutor = nullptr;
    Super::Deinitialize();
}
