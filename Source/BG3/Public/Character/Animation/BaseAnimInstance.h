#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHitNotifyBegin);
class USkillTaskPlayMontage;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
    class UAnimMontage* AttackMontage;

    FOnHitNotifyBegin OnHitNotifyBegin;

    void SetActiveMontageTask(USkillTaskPlayMontage* Task);
    USkillTaskPlayMontage* GetActiveMontageTask() const;

    void AnimNotify_Hit();

private:
    UPROPERTY()
    TWeakObjectPtr<USkillTaskPlayMontage> ActiveMontageTask;
};
