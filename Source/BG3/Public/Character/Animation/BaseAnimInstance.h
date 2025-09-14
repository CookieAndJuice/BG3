
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHitNotifyBegin);

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

	// 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
	class UAnimMontage* AttackMontage;
	
	// 공격 애니메이션 재생 함수
	void PlayAttackAnimation();

	// 애님 노티파이
	FOnHitNotifyBegin OnHitNotifyBegin;

	void AnimNotify_Hit();
};
