
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ActionBudgetProvider.h"
#include "BaseCharacter.generated.h"

enum class EActionCost : uint8;
class UCharacterArchetype;
class USkillSet;
class USkillBookComponent;
class UCharacterStatsComponent;
class UTurnWidgetManagerComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UTurnWidgetManagerComponent> TurnWidgetManager;
	
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

	
	/* 애님 몽타주 */

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayAttackAnimation(UAnimMontage* MontageToPlay);

	void SetID(int32 id);
	int32 GetID();

	void SetPortrait(UTexture2D* portrait);
	UTexture2D* GetPortrait();

	void SetIsHit(bool bInHit);
	bool GetIsHit();
	void SetIsDead(bool bInDead);
	bool GetIsDead();
	
protected:
	virtual void BeginPlay() override;
	void GrantSkills();

	// Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Weapon;

private:
	int32 CharacterID = 0;
	
	// Character Portrait for UI
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	UTexture2D* CharacterPortrait = nullptr;

	/* 현재 행동력 */
	int32 CurrentActions = 1;
	int32 CurrentBonusActions = 1;
	int32 CurrentReactions = 1;

	/* 애니메이션 */
	
	bool bIsHit = false;
	bool bIsDead = false;
	
};
