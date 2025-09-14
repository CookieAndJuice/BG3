#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/Tasks/SkillTaskBase.h"
#include "Animation/AnimMontage.h"
#include "SkillTaskPlayMontage.generated.h"

UCLASS()
class BG3_API USkillTaskPlayMontage : public USkillTaskBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
    TWeakObjectPtr<UAnimMontage> Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
    FName NotifyToWaitFor = NAME_None; // Optional: wait specific notify

    virtual void Start(UObject* WorldContext, AActor* Caster, const class USkillDefinition* Skill, const TArray<AActor*>& Targets) override;
    virtual void Cancel() override;
};
