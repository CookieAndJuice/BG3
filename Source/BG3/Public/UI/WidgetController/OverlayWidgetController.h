
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UI/Widget/ActionSlotEntry.h" // For FActionSlotView used in delegate
#include "OverlayWidgetController.generated.h"

class ABaseCharacter;
class USkillBookComponent;
class USkillDefinition;
class UCharacterStatsComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionSlotsUpdated, const TArray<FActionSlotView>&, Slots);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatsInitialized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChangedUI, float, Current, float, Max);


UCLASS()
class BG3_API UOverlayWidgetController : public UObject
{
    GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Combat|UI")
	void Initialize(ABaseCharacter* InCharacter);

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

    UFUNCTION(Category="Combat|UI")
    void RequestUseSkill(int32 SkillID);

	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> OwningCharacter;

    UPROPERTY()
    USkillBookComponent* SkillBook = nullptr;

    UPROPERTY()
    UCharacterStatsComponent* Stats = nullptr;

    void BuildAndBroadcast();

	UFUNCTION()
	void HandleCooldownChanged(const USkillDefinition* Skill, int32 NewRounds);

    UFUNCTION()
    void HandleUsabilityChanged(const USkillDefinition* Skill, bool bUsable);

    // Stats handlers
    UFUNCTION()
    void HandleHealthChanged(float NewHealth, float MaxHealth);

    UFUNCTION()
    void HandleManaChanged(float NewMana, float MaxMana);

private:
    
};
