#include "Component/MouseInputComponent.h"
#include "BG3/BG3.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Character/BaseCharacter.h"
#include "Component/CharacterStatsComponent.h"
#include "Controller/BG3GameModePlayerController.h"
#include "Game/BG3GameManageSubsystem.h"
#include "Game/BG3GameState.h"
#include "Game/SkillExecutionSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"

void UMouseInputComponent::ClearMoveIndicator()
{
    if (ActiveMoveIndicator.IsValid())
    {
        ActiveMoveIndicator->Destroy();
        ActiveMoveIndicator = nullptr;
    }
}

UMouseInputComponent::UMouseInputComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UMouseInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!PendingTurnMove.bActive)
    {
        return;
    }

    ABG3GameModePlayerController* PC = Cast<ABG3GameModePlayerController>(GetOwner());
    if (!PC)
    {
        return;
    }

    ABaseCharacter* Character = PC->PossessedCharacter;
    if (!Character || !Character->Stats)
    {
        return;
    }

    const float DeltaDistance = PendingTurnMove.ConsumeToLocation(Character->GetActorLocation());
    if (DeltaDistance <= KINDA_SMALL_NUMBER)
    {
        return;
    }

    Character->Stats->ConsumeMoveDistance(DeltaDistance);

    if (!PendingTurnMove.bActive)
    {
        SetComponentTickEnabled(false);
    }
}

void UMouseInputComponent::BindInput(UEnhancedInputComponent* EIC)
{
    if (!EIC)
    {
        return;
    }

    // ?≪뀡 ?먯뀑???ㅼ젙??寃쎌슦?먮쭔 諛붿씤??
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
    if (!PC)
    {
        return;
    }

    FHitResult Hit;
    const bool bHit = PC->GetHitResultUnderCursor(ECC_Visibility, true, Hit);

    // 스킬 타겟팅 중일 때
    if (!IsIdle())
    {
        if (Hit.Component->GetCollisionObjectType() != ECC_Pawn) return;
        if (USkillExecutionSubsystem* SES = GetWorld()->GetSubsystem<USkillExecutionSubsystem>())
        {
            SES->OnClickInTargeting(Hit);
        }
        return;
    }

    // 스킬 타겟팅 중이 아닐 때
    if (UBG3GameManageSubsystem* Sub = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>())
    {
        if (ABaseCharacter* Character = Sub->GetCurrentPawn())
        {
            ClearMoveIndicator();
            
            if (Character->Stats->RemainingMoveDistance > 100.f)
            {
                
                FTransform IndicatorTransform;
                FVector IndicatorLocation = Hit.ImpactPoint;
                IndicatorLocation.Z += 60.f;
                IndicatorTransform.SetLocation(IndicatorLocation);

                FActorSpawnParameters Params;
                Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                
                ActiveMoveIndicator = GetWorld()->SpawnActor<AActor>(MoveIndicatorClass, IndicatorTransform, Params);
            }
            

            PRINTLOG(TEXT("RemainingMoveDistance : %.1f"), Character->Stats->RemainingMoveDistance);
            IssueTurnMove(Hit.Location);
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
            if (ABG3GameState* GState = World->GetGameState<ABG3GameState>())
            {
                SES->ConfirmAndExecute(GState->GetCurrentRound());
            }
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

void UMouseInputComponent::IssueTurnMove(const FVector& DesiredLocation)
{
    ABG3GameModePlayerController* PC = Cast<ABG3GameModePlayerController>(GetOwner());
    if (!PC) return;

    ABaseCharacter* Character = PC->PossessedCharacter;
    if (!Character || !Character->Stats) return;

    ApplyTravelledDistance(*Character);

    const float RemainingDistance = Character->Stats->RemainingMoveDistance;
    const FTurnMoveResult MoveResult = GetClampedTurnDistance(GetWorld(), Character->GetActorLocation(), DesiredLocation, RemainingDistance);
    if (MoveResult.ConsumedDistance <= 0) return;
    FVector Goal = MoveResult.TargetLocation;

    if (AAIController* AIController = Cast<AAIController>(Character->GetController()))
    {
        FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalLocation(Goal);
        MoveRequest.SetAcceptanceRadius(25.f);
        MoveRequest.SetAllowPartialPath(true);
        MoveRequest.SetProjectGoalLocation(true);

        FNavPathSharedPtr OutPath;
        const FPathFollowingRequestResult RequestResult = AIController->MoveTo(MoveRequest, &OutPath);

        if (RequestResult.Code != EPathFollowingRequestResult::RequestSuccessful)
        {
            PendingTurnMove.Reset();
            SetComponentTickEnabled(false);

            if (RequestResult.Code == EPathFollowingRequestResult::AlreadyAtGoal)
            {
                Character->Stats->BroadcastMoveDistance();
                Character->Stats->ConsumeMoveDistance(RemainingDistance);
            }
            return;
        }

        PendingTurnMove.Begin(Character->GetActorLocation(), MoveRequest.GetGoalLocation(), MoveResult.ConsumedDistance);
        SetComponentTickEnabled(true);

        if (UPathFollowingComponent* PathComp = AIController->GetPathFollowingComponent())
        {
            PathComp->OnRequestFinished.RemoveAll(this);
            PathComp->OnRequestFinished.AddUObject(this, &UMouseInputComponent::HandleMoveCompleted);
        }
    }
}

void UMouseInputComponent::HandleMoveCompleted(FAIRequestID /*RequestID*/, const FPathFollowingResult& /*Result*/)
{
    if (ABG3GameModePlayerController* PC = Cast<ABG3GameModePlayerController>(GetOwner()))
    {
        if (ABaseCharacter* Character = PC->PossessedCharacter)
        {
            ApplyTravelledDistance(*Character);
        }
    }
}

void UMouseInputComponent::ApplyTravelledDistance(ABaseCharacter& Character)
{
    if (!Character.Stats)
    {
        PendingTurnMove.Reset();
        SetComponentTickEnabled(false);
        return;
    }

    const float PlannedDistance = PendingTurnMove.PlannedDistance;
    const float FinalDelta = PendingTurnMove.Flush(Character.GetActorLocation());
    if (FinalDelta >= 0)
    {
        Character.Stats->ConsumeMoveDistance(FinalDelta);
    }

    const float ConsumedDistance = PlannedDistance - PendingTurnMove.RemainingBudget;
    if (ConsumedDistance > 0)
    {
        const float RemainingMove = Character.Stats->GetRemainingMoveDistance();
        const float MaxMove = Character.Stats->GetMaxMoveDistanceValue();
        PRINTLOG(TEXT("[Move] %s travelled %.1f (planned %.1f). Remaining %.1f / %.1f"),
            *Character.GetName(), ConsumedDistance, PlannedDistance, RemainingMove, MaxMove);
    }

    if (PendingTurnMove.bActive == true)
    {
        ClearMoveIndicator();
    }
    

    PendingTurnMove.Reset();
    SetComponentTickEnabled(false);
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

FTurnMoveResult UMouseInputComponent::GetClampedTurnDistance(UWorld* World, const FVector& Start, const FVector& DesiredEnd, float RemainingDistance)
{
    FTurnMoveResult Result;

    if (!World || RemainingDistance <= 0.f)
    {
        Result.TargetLocation = Start;
        return Result;
    }

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
    if (!NavSys)
    {
        Result.TargetLocation = Start;
        return Result;
    }

    UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(World, Start, DesiredEnd);
    if (!NavPath || NavPath->PathPoints.Num() < 2)
    {
        Result.TargetLocation = Start;
        return Result;
    }

    const TArray<FVector>& Points = NavPath->PathPoints;
    float Accumulated = 0.f;

    for (int32 Index = 0; Index < Points.Num() - 1; ++Index)
    {
        const FVector SegmentStart = Points[Index];
        const FVector SegmentEnd = Points[Index + 1];
        const float SegmentLength = FVector::Dist(SegmentStart, SegmentEnd);

        if (Accumulated + SegmentLength <= RemainingDistance)
        {
            Accumulated += SegmentLength;
            continue;
        }

        const float DistanceLeft = RemainingDistance - Accumulated;
        const FVector Direction = (SegmentEnd - SegmentStart).GetSafeNormal();
        Result.TargetLocation = SegmentStart + Direction * DistanceLeft;
        Result.ConsumedDistance = RemainingDistance;
        return Result;
    }

    Result.TargetLocation = Points.Last();
    Result.ConsumedDistance = Accumulated;
    Result.bWithinAllowance = true;
    return Result;
}
