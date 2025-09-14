#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillTaskBase.generated.h"

DECLARE_DELEGATE(FOnTaskFinished);
DECLARE_DELEGATE_OneParam(FOnTaskFailed, FName /*Reason*/);

UCLASS(Abstract)
class BG3_API USkillTaskBase : public UObject
{
    GENERATED_BODY()
public:
    virtual void Start(UObject* WorldContext, AActor* Caster, const class USkillDefinition* Skill, const TArray<AActor*>& Targets) PURE_VIRTUAL(USkillTaskBase::Start, );
    virtual void Cancel() {}

    void Bind(const FOnTaskFinished& InOnFinished, const FOnTaskFailed& InOnFailed)
    {
        OnFinished = InOnFinished;
        OnFailed = InOnFailed;
    }

protected:
    FOnTaskFinished OnFinished;
    FOnTaskFailed OnFailed;
};
