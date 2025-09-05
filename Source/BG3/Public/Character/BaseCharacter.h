
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCharacterArchetype;
class USkillSet;
class USkillBookComponent;

UCLASS()
class BG3_API ABaseCharacter : public ACharacter
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

	


protected:
	virtual void BeginPlay() override;
	void GrantSkills();
	
};
