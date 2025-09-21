#include "Component/CharacterStatsComponent.h"
#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Character/BG3EnemyCharacter.h"
#include "Component/SimpleEnemyFSMComponent.h"
#include "Game/BG3GameManageSubsystem.h"
#include "Game/BG3GameMode.h"
#include "GameFramework/Actor.h"

UCharacterStatsComponent::UCharacterStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterStatsComponent::ResetRemainingMoveDistance()
{
    RemainingMoveDistance = MaxMoveDistance;
    BroadcastMoveDistance();
}

void UCharacterStatsComponent::ConsumeMoveDistance(float Distance)
{
    RemainingMoveDistance = RemainingMoveDistance - Distance;
    BroadcastMoveDistance();
}

void UCharacterStatsComponent::BroadcastMoveDistance() const
{
    OnMoveDistanceChanged.Broadcast(RemainingMoveDistance, MaxMoveDistance);
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
    BroadcastMoveDistance();

    Character = Cast<ABaseCharacter>(GetOwner());
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
    const FString Msg = FString::Printf(TEXT("Damaged %s | HP: %.1f -> %.1f / %.1f"), *DamagedActor->GetName(), Prev, Health,
  MaxHealth);
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, Msg);
    
    // Notify UI
    OnHealthChanged.Broadcast(Health, MaxHealth);

    if (Health <= 0.f)
    {
        PRINTLOG(TEXT("%s has died"), *DamagedActor->GetName());
        Character->SetIsDead(true);
        
        auto* Subsystem = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>();
        Subsystem->RemoveCharacterFromCombatPawns(Character);
        
        ABG3GameMode* GM = Cast<ABG3GameMode>(GetWorld()->GetAuthGameMode());
        EResultState result = GM->DecideWhoWin();
        
        // Enemy가 이겼을 때
        if (result == EResultState::Enemy)
        {
            OnFadeOut.Broadcast(result);
            GM->StopEnemies();
            PRINTDELEGATELOG(TEXT("Enemy Win 1"));
        } // Player가 이겼을 때
        else if (result == EResultState::Player)
        {
            OnFadeOut.Broadcast(result);
            GM->StopEnemies();
            PRINTDELEGATELOG(TEXT("Player Win 1"));
        }

        
        
        
    }
}




