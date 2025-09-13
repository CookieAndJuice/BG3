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
        CastingStarted.Unbind();
        CastingStarted.BindUObject(SkillExecutor, &USkillCastExecutor::OnCastingStarted);
    }
}

void USkillExecutionSubsystem::Deinitialize()
{
    CastingStarted.Unbind();
    SkillExecutor = nullptr;
    Super::Deinitialize();
}

