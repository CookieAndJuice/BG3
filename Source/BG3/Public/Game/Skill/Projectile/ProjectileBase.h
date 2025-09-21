
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

UCLASS()
class BG3_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();
	void Init(ABaseCharacter* Caster, ABaseCharacter* Target, USkillDefinition* Skill);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* Box;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCameraShakeBase> ImpactCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraComponent* Trail;

	void OnImpactFX(const FHitResult& Hit);
	void LaunchInDirection(const FVector& Dir);

	UFUNCTION()
	void OnBoxComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


public:
	virtual void Tick(float DeltaTime) override;

	int32 Damage;
};
