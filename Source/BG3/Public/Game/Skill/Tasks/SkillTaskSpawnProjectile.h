#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/Tasks/SkillTaskBase.h"
#include "SkillTaskSpawnProjectile.generated.h"

UCLASS()
class BG3_API USkillTaskSpawnProjectile : public USkillTaskBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
    TSubclassOf<AActor> ProjectileClass;

    virtual void Start(UObject* WorldContext, AActor* Caster, const class USkillDefinition* Skill, const TArray<AActor*>& Targets) override;
    virtual void Cancel() override;
};
