#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHitNotifyBegin);
class USkillTaskPlayMontage;
class ABaseCharacter;

UCLASS()
class BG3_API UBaseAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
    float Speed = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
    float Direction = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
    bool IsInAir = false;

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    virtual void NativeBeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
    class UAnimMontage* AttackMontage;

    FOnHitNotifyBegin OnHitNotifyBegin;

    void SetActiveMontageTask(USkillTaskPlayMontage* Task);
    USkillTaskPlayMontage* GetActiveMontageTask() const;

    UFUNCTION()
    void AnimNotify_Hit();

    UFUNCTION()
    void AnimNotify_HitEnd();

    UFUNCTION()
    void AnimNotify_Fire();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bIsHit = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bIsDead = false;

private:
    UPROPERTY()
    TWeakObjectPtr<USkillTaskPlayMontage> ActiveMontageTask;

    ABaseCharacter* Character;
};
