#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "MouseInputComponent.generated.h"

class UInputAction;
class UEnhancedInputComponent;
class AActor;
class ABaseCharacter;
struct FAIRequestID;
struct FPathFollowingResult;

struct FTurnMoveResult
{
    FVector TargetLocation;
    float ConsumedDistance = 0.f;
    bool bWithinAllowance = false;
};

USTRUCT()
struct FPendingTurnMove
{
    GENERATED_BODY()

    FVector StartLocation = FVector::ZeroVector;
    FVector PlannedDestination = FVector::ZeroVector;
    FVector LastLocation = FVector::ZeroVector;
    float PlannedDistance = 0.f;
    float RemainingBudget = 0.f;
    bool bActive = false;

    void Begin(const FVector& InStart, const FVector& InDestination, float InDistance)
    {
        StartLocation = InStart;
        PlannedDestination = InDestination;
        LastLocation = InStart;
        PlannedDistance = InDistance;
        RemainingBudget = InDistance;
        bActive = RemainingBudget > KINDA_SMALL_NUMBER;
    }

    float ConsumeToLocation(const FVector& CurrentLocation)
    {
        if (!bActive)
        {
            return 0.f;
        }

        const FVector Delta = CurrentLocation - LastLocation;
        const float Step = Delta.Size2D();
        LastLocation = CurrentLocation;

        if (Step <= KINDA_SMALL_NUMBER || RemainingBudget <= KINDA_SMALL_NUMBER)
        {
            RemainingBudget = FMath::Max(0.f, RemainingBudget);
            return 0.f;
        }

        const float Allowed = FMath::Min(Step, RemainingBudget);
        RemainingBudget = FMath::Max(0.f, RemainingBudget - Allowed);

        if (RemainingBudget <= KINDA_SMALL_NUMBER)
        {
            RemainingBudget = 0.f;
            bActive = false;
        }

        return Allowed;
    }

    float Flush(const FVector& CurrentLocation)
    {
        return ConsumeToLocation(CurrentLocation);
    }

    void Reset()
    {
        bActive = false;
        PlannedDistance = 0.f;
        RemainingBudget = 0.f;
        StartLocation = FVector::ZeroVector;
        PlannedDestination = FVector::ZeroVector;
        LastLocation = FVector::ZeroVector;
    }
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BG3_API UMouseInputComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMouseInputComponent();

    void BindInput(UEnhancedInputComponent* EIC);
    void OnClick(const FInputActionValue& Value);
    void OnConfirm(const FInputActionValue& Value);
    void OnCancel(const FInputActionValue& Value);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Move")
    TSubclassOf<AActor> MoveIndicatorClass;

    TWeakObjectPtr<AActor> ActiveMoveIndicator;

    FPendingTurnMove PendingTurnMove;

    void IssueTurnMove(const FVector& DesiredLocation);
    void HandleMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
    void ApplyTravelledDistance(ABaseCharacter& Character);

protected:
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> ClickAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> ConfirmAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> CancelAction;

private:
    AActor* GetActorUnderCursor() const;
    bool IsTargeting() const;
    bool IsIdle() const;

    FTurnMoveResult GetClampedTurnDistance(UWorld* World, const FVector& Start, const FVector& DesiredEnd, float RemainingDistance);
};
