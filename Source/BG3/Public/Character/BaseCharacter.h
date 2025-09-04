
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCharacterArchetype;
class USkillSet;

UCLASS()
class BG3_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skills|Defaults")
	TObjectPtr<USkillSet> DefaultSkills;


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
