#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "Game/Skill/Tasks/SkillTaskBase.h"
#include "SkillTaskPlayMontage.generated.h"

class ABaseCharacter;
class UBaseAnimInstance;
class USkillDefinition;
class UNiagaraSystem;
class UNiagaraSystemComponent;
class USoundBase;

DECLARE_MULTICAST_DELEGATE(FOnAnimEnded);

UCLASS()
class BG3_API USkillTaskPlayMontage : public USkillTaskBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
    UAnimMontage* Montage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
    FName HitNotifyName = FName(TEXT("Hit"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
    USkillDefinition* SkillInfo = nullptr;

    UFUNCTION(BlueprintCallable, Category="Montage")
    void SetRound(int32 InRound) { Round = InRound; }

    void HandleHitNotify(FName TriggeredNotify = NAME_None);
    void HandleShootNotify(FName TriggeredNotify = NAME_None);

    virtual void Start(UObject* WorldContext, AActor* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets) override;
    virtual void Cancel() override;
    void FinalizeCast();

    FOnAnimEnded OnAnimEnded;

private:
    void ClearActiveTaskBinding();

    UFUNCTION()
    void OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted);

    UFUNCTION()
    void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);

private:
    UPROPERTY()
    TWeakObjectPtr<ABaseCharacter> CasterCharacter;

    UPROPERTY()
    TWeakObjectPtr<UAnimInstance> AnimInst;

    UPROPERTY()
    TArray<TWeakObjectPtr<AActor>> WeakTargets;

    UPROPERTY()
    UNiagaraSystem* NiagaraSystem;

    UPROPERTY()
    USoundBase* HitSound;
    
    UPROPERTY()
    int32 Round = 0;

    bool bHitApplied = false;
    bool bShootHandled = false;
    
};

