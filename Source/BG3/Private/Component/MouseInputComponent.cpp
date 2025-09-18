#include "Component/MouseInputComponent.h"

#include "BG3/BG3.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/BaseCharacter.h"
#include "Game/SkillExecutionSubsystem.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "Game/BG3GameManageSubsystem.h"
#include "Game/BG3GameState.h"
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
    const APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC) return;
    FHitResult Hit;
    const bool bHit = PC->GetHitResultUnderCursor(ECC_Visibility, true, Hit);

    // 타겟팅 중일 때
    if (!IsIdle())
    {
        if (USkillExecutionSubsystem* SES = GetWorld()->GetSubsystem<USkillExecutionSubsystem>())
        {
            SES->OnClickInTargeting(Hit);
            
        }
    }
    else // 타겟팅 중이 아닐 때 (이동)
    {
        if (UBG3GameManageSubsystem* Sub = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>())
        {
            if (ABaseCharacter* Character = Sub->GetCurrentPawn())
            {
                FTransform T;
                FVector loc = Hit.ImpactPoint;
                loc.Z += 60.f;
                T.SetLocation(loc);
                FActorSpawnParameters Params;
                Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                if (ActiveMoveIndicator.IsValid())
                {
                    ActiveMoveIndicator->Destroy();
                    ActiveMoveIndicator = nullptr;
                }
                
                ActiveMoveIndicator = GetWorld()->SpawnActor<AActor>(MoveIndicatorClass, T, Params);
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(Character->GetController(), Hit.Location);
            }
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
            ABG3GameState* GState = GetWorld()->GetGameState<ABG3GameState>();
            if (!GState) return;
            SES->ConfirmAndExecute(GState->GetCurrentRound());
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

bool UMouseInputComponent::IsIdle() const
{
    if (const UWorld* World = GetWorld())
    {
        if (const USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
        {
            return SES->GetCastState() == ECastState::Idle;
        }
    }
    return false;
}
