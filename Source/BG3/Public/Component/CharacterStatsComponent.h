#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSig, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChangedSig, float, NewMana, float, MaxMana);


UCLASS(ClassGroup=(BG3), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class BG3_API UCharacterStatsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCharacterStatsComponent();

    // Current / Max Health
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats|Health")
    float MaxHealth = 100.f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Stats|Health")
    float Health = 0.f;

    // Current / Max Mana (kept for future use)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats|Mana")
    float MaxMana = 50.f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Stats|Mana")
    float Mana = 0.f;

    // Move
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats|Mana")
    float MoveAllowanceCm = 750.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats|Mana")
    float RemainingMoveCm = 750.f;
    

    // UI delegates
    UPROPERTY(BlueprintAssignable, Category="Stats|Events")
    FOnHealthChangedSig OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category="Stats|Events")
    FOnManaChangedSig OnManaChanged;

public:
    UFUNCTION(BlueprintCallable, Category="Stats|Health")
    FORCEINLINE float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable, Category="Stats|Health")
    FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintCallable, Category="Stats|Mana")
    FORCEINLINE float GetMana() const { return Mana; }

    UFUNCTION(BlueprintCallable, Category="Stats|Mana")
    FORCEINLINE float GetMaxMana() const { return MaxMana; }

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
