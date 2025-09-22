
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USkillDefinition;
class UBoxComponent;
class UStaticMeshComponent;
class UCameraShakeBase;
class ABaseCharacter;
class USoundBase;

UCLASS()
class BG3_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();
	void Init(ABaseCharacter* Caster, ABaseCharacter* Target, const USkillDefinition* Skill, const FVector& LaunchDirection);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* Box;
	
protected:
	virtual void BeginPlay() override;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* Scene;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCameraShakeBase> ImpactCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraComponent* Trail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraComponent* Niagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound;

	
	float DistanceToPlayer;

	UPROPERTY()
	ABaseCharacter* TargetCharacter;

	UPROPERTY()
	ABaseCharacter* CasterCharacter;

	void OnImpactFX(const FHitResult& Hit);
	void LaunchInDirection(const FVector& Dir);

	UFUNCTION()
	void OnBoxComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	

public:
	virtual void Tick(float DeltaTime) override;

	int32 Damage;

private:
	void FinalizeCast(ABaseCharacter* CasterCharacter, ABaseCharacter* TargetCharacter);
};


