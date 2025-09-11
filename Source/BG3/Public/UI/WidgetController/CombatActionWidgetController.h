
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UI/Widget/ActionSlotEntry.h"
#include "CombatActionWidgetController.generated.h"

class ABaseCharacter;
class USkillBookComponent;
class USkillDefinition;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionSlotsUpdated, const TArray<FActionSlotView>&, Slots);

UCLASS()
class BG3_API UCombatActionWidgetController : public UObject
{
	GENERATED_BODY() 

public:
	UFUNCTION(BlueprintCallable, Category="Combat|UI")
	void Initialize(ABaseCharacter* InCharacter);

	UFUNCTION(BlueprintCallable, Category="Combat|UI")
	void RefreshSlots();

	//UPROPERTY(BlueprintAssignable, Category="Combat|UI")
	//FOnActionSlotsUpdated OnActionSlotsUpdated;

	UFUNCTION(Category="Combat|UI")
	void RequestUseSkill(int32 SkillID);

private:
	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> OwningCharacter;

	UPROPERTY()
	USkillBookComponent* SkillBook = nullptr;

	void BuildAndBroadcast();

	UFUNCTION()
	void HandleCooldownChanged(const USkillDefinition* Skill, int32 NewRounds);

	UFUNCTION()
	void HandleUsabilityChanged(const USkillDefinition* Skill, bool bUsable);
};
