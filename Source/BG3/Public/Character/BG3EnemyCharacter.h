
#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BG3EnemyCharacter.generated.h"


class USimpleEnemyFSMComponent;


UCLASS()
class BG3_API ABG3EnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABG3EnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	// FSM
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	USimpleEnemyFSMComponent* EnemyFSMComp;
	
	virtual void SetMyTurn();

	

private:// Attack Target
	UPROPERTY()
	TObjectPtr<class ABaseCharacter> AttackTarget;

	
	
};
