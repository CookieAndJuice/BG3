#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/Tasks/SkillTaskBase.h"
#include "AIController.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "SkillTaskMoveTo.generated.h"

UCLASS()
class BG3_API USkillTaskMoveTo : public USkillTaskBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move")
    float AcceptRadius = 150.f; // cm

    virtual void Start(UObject* WorldContext, AActor* Caster, const class USkillDefinition* Skill, const TArray<AActor*>& Targets) override;
    virtual void Cancel() override;

private:
    UFUNCTION()
    void HandleMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

    UPROPERTY()
    TWeakObjectPtr<class AAIController> CachedController;

    // Track last move request to filter callbacks
    FAIRequestID LastRequestId;
};
