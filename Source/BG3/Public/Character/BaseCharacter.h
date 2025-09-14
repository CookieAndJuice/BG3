
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ActionBudgetProvider.h"
#include "BaseCharacter.generated.h"

class UCharacterArchetype;
class USkillSet;
class USkillBookComponent;
class UCharacterStatsComponent;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	None,
	Idle, Move,
	Action, BonusAction, ReAction,
	OpportunityAttack,
	Hit, Die
};

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skills|Defaults")
	TObjectPtr<USkillSet> ClassSkills;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USkillBookComponent> SkillBook;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UCharacterStatsComponent> Stats;
	
	/* 행동력 */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions")
	int32 ActionCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions")
	int32 BonusActionCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions")
	int32 ReactionCount = 1;

	bool bIsInAction;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ActionBudget")
	bool CanSpendActionSlot(EActionCost Cost) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ActionBudget")
	void SpendActionSlot(EActionCost Cost);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ActionBudget")
	void RefundActionSlot(EActionCost Cost);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ActionBudget")
	void BeginTurnReset();
	

protected:
	virtual void BeginPlay() override;
	void GrantSkills();

private:

	/* 현재 행동력 */
	int32 CurrentActions = 1;
	int32 CurrentBonusActions = 1;
	int32 CurrentReactions = 1;
	
public: // FSM
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	TObjectPtr<class UFSMComponent> FSMComp;

protected:
	void BuildFSM();
};
