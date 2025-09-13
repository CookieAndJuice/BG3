#include "Component/CharacterStatsComponent.h"

#include "BG3/BG3.h"
#include "GameFramework/Actor.h"

UCharacterStatsComponent::UCharacterStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterStatsComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initialize current values
    Health = FMath::Clamp(Health <= 0.f ? MaxHealth : Health, 0.f, MaxHealth);
    Mana   = FMath::Clamp(Mana   <= 0.f ? MaxMana   : Mana,   0.f, MaxMana);

    if (AActor* Owner = GetOwner())
    {
        // Bind to damage: will fire whenever UGameplayStatics::ApplyDamage is called on this actor
        Owner->OnTakeAnyDamage.AddDynamic(this, &UCharacterStatsComponent::HandleTakeAnyDamage);
    }

    // Push initial values to UI
    OnHealthChanged.Broadcast(Health, MaxHealth);
    OnManaChanged.Broadcast(Mana, MaxMana);
}

void UCharacterStatsComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.f || !DamagedActor)
    {
        return;
    }

    const float Prev = Health;
    Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);

    // Debug log: print target HP after damage
    PRINTLOG(TEXT("Damaged %s | HP: %.1f -> %.1f / %.1f"), *DamagedActor->GetName(), Prev, Health, MaxHealth);

    // Notify UI
    OnHealthChanged.Broadcast(Health, MaxHealth);

    if (Health <= 0.f)
    {
        PRINTLOG(TEXT("%s has died"), *DamagedActor->GetName());
    }
}
