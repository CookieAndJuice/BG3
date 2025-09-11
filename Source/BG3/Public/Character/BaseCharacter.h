
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ActionBudgetProvider.h"
#include "BaseCharacter.generated.h"

class UCharacterArchetype;
class USkillSet;
class USkillBookComponent;

UCLASS()
class BG3_API ABaseCharacter : public ACharacter, public IActionBudgetProvider
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skills|Defaults")
	TObjectPtr<USkillSet> DefaultSkills;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkillBookComponent> SkillBook;

	/* 행동력 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ActionBudget")
	bool CanSpendActionSlot(EActionCost Cost) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ActionBudget")
	void SpendActionSlot(EActionCost Cost);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ActionBudget")
	void RefundActionSlot(EActionCost Cost);

protected:
	virtual void BeginPlay() override;
	void GrantSkills();

private:

	/* 행동력 */
	int32 Actions = 1;
	int32 BonusActions = 1;
	int32 Reactions = 1;
	
	
	
};
