#include "Component/MouseInputComponent.h"

#include "BG3/BG3.h"
#include "Game/SkillExecutionSubsystem.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"

UMouseInputComponent::UMouseInputComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMouseInputComponent::BindInput(UEnhancedInputComponent* EIC)
{
    if (!EIC)
    {
        return;
    }

    // 액션 에셋이 설정된 경우에만 바인딩
    if (ConfirmAction)
    {
        EIC->BindAction(ConfirmAction, ETriggerEvent::Triggered, this, &UMouseInputComponent::OnConfirm);
    }
    if (CancelAction)
    {
        EIC->BindAction(CancelAction, ETriggerEvent::Triggered, this, &UMouseInputComponent::OnCancel);
    }
}

void UMouseInputComponent::OnClick(const FInputActionValue& /*Value*/)
{
    if (!IsTargeting()) return;
    

    if (UWorld* World = GetWorld())
    {
        if (USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
        {
            const APlayerController* PC = Cast<APlayerController>(GetOwner());
            if (!PC) return;
            
            FHitResult Hit;
            const bool bHit = PC->GetHitResultUnderCursor(ECC_Visibility, true, Hit);

            /*
            if (AActor* HitActor = GetActorUnderCursor())
            {
                TArray<AActor*> Targets;
                Targets.Add(HitActor);
                SES->SetTargets(Targets);
                PRINTLOG(TEXT("Target Set : %s"), *HitActor->GetName());
                // TODO: CurrentRound 입력하기
                SES->ConfirmAndExecute( 0);
            }
            */
        }
    }
}

void UMouseInputComponent::OnConfirm(const FInputActionValue& /*Value*/)
{
    if (!IsTargeting())
    {
        return;
    }

    if (UWorld* World = GetWorld())
    {
        if (USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
        {
            // TODO: 턴 시스템 완성 시 실제 CurrentRound 값 전달
            SES->ConfirmAndExecute(/*CurrentRound*/ 0);
        }
    }
}

void UMouseInputComponent::OnCancel(const FInputActionValue& /*Value*/)
{
    if (UWorld* World = GetWorld())
    {
        if (USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
        {
            SES->CancelCast();
        }
    }
}

AActor* UMouseInputComponent::GetActorUnderCursor() const
{
    const APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC)
    {
        return nullptr;
    }

    FHitResult Hit;
    const bool bHit = PC->GetHitResultUnderCursor(ECC_Pawn, true, Hit);
    
    if (bHit && Hit.GetActor())
    {
        return Hit.GetActor();
    }
    return nullptr;
}

bool UMouseInputComponent::IsTargeting() const
{
    if (const UWorld* World = GetWorld())
    {
        if (const USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
        {
            return SES->GetCastState() == ECastState::Targeting;
        }
    }
    return false;
}
