
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UI/Widget/ActionSlotEntry.h" // For FActionSlotView used in delegate
#include "OverlayWidgetController.generated.h"

class ABaseCharacter;
class USkillBookComponent;
class USkillDefinition;
class UCharacterStatsComponent;
class ABG3GameModePlayerController;
enum class EResultState : uint8; 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionSlotsUpdated, const TArray<FActionSlotView>&, Slots);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatsInitialized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChangedUI, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMoveDistanceChangedUI, float, Remaining, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFadeOutAnimationStart, EResultState, result);


UCLASS()
class BG3_API UOverlayWidgetController : public UObject
{
    GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Combat|UI")
	void Initialize(ABaseCharacter* InCharacter, ABG3GameModePlayerController* PC);

    UFUNCTION(BlueprintCallable, Category="Combat|UI")
    void RefreshSlots();

    UPROPERTY(BlueprintAssignable, Category="Combat|UI")
    FOnActionSlotsUpdated OnActionSlotsUpdated;

    // Stats events for UI (HP/MP)
    UPROPERTY(BlueprintAssignable, Category="Stats|UI")
    FOnStatsInitialized OnStatsInitialized; // optional hook if needed

    UPROPERTY(BlueprintAssignable, Category="Stats|UI")
    FOnStatChangedUI OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category="Stats|UI")
    FOnStatChangedUI OnManaChanged;

    UPROPERTY(BlueprintAssignable, Category="Stats|UI")
    FOnMoveDistanceChangedUI OnMoveDistanceChanged;

	FOnFadeOutAnimationStart OnFadeOutAnimationStart;

    UFUNCTION(Category="Combat|UI")
    void RequestUseSkill(int32 SkillID);

	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> OwningCharacter;

    UPROPERTY()
    USkillBookComponent* SkillBook = nullptr;

    UPROPERTY()
    UCharacterStatsComponent* Stats = nullptr;

	UPROPERTY()
	USkillDefinition* TargetingSkill = nullptr;

    void BuildAndBroadcast();

	UFUNCTION()
	void HandleCooldownChanged(const USkillDefinition* Skill, int32 NewRounds);

    UFUNCTION()
    void HandleUsabilityChanged(const USkillDefinition* Skill, bool bUsable);

	UFUNCTION()
	void HandleFadeOut(EResultState result);

	UFUNCTION()
	void HandleCastStarted(ABaseCharacter* Character, const USkillDefinition* Skill);

	UFUNCTION()
	void HandleCastCanceled();

	UFUNCTION()
	void HandleCastResolved(const FSkillResult& SkillResult);

    // Stats handlers
    UFUNCTION()
    void HandleHealthChanged(float NewHealth, float MaxHealth);

    UFUNCTION()
    void HandleManaChanged(float NewMana, float MaxMana);

    UFUNCTION()
    void HandleMoveDistanceChanged(float Remaining, float Max);

private:
    void BindAllFadeOutSources();
    void ClearFadeOutBindings();

    TSet<TWeakObjectPtr<class UCharacterStatsComponent>> FadeOutSources;
};





