#include "Game/Skill/Tasks/SkillTaskSpawnProjectile.h"

#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Data/SkillDefinition.h"
#include "Engine/World.h"
#include "Game/Skill/Projectile/ProjectileBase.h"

void USkillTaskSpawnProjectile::Start(UObject* WorldContext, AActor* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets)
{
    if (!WorldContext || !Caster || !Skill) return;

    ABaseCharacter* Character = Cast<ABaseCharacter>(Caster);
    if (!Character) return;

    const TSubclassOf<AProjectileBase> ProjectileClass = Skill->Damage.ProjectileClass;
    if (!ProjectileClass) return;

    const FVector SocketLocation = Character->GetMesh()->GetSocketLocation(Skill->Damage.SpawnSocketName);

    if (Targets.Num() == 0)
    {
        const FVector FallbackDir = Character->GetActorForwardVector().GetSafeNormal();
        if (!FallbackDir.IsNearlyZero())
        {
            if (AProjectileBase* Projectile = Character->GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SocketLocation, FallbackDir.Rotation()))
            {
                Projectile->Init(Character, nullptr, Skill, FallbackDir);
            }
        }
    }
    else
    {
        for (AActor* Target : Targets)
        {
            ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(Target);
            if (!TargetCharacter) continue;

            const FVector AimLocation = TargetCharacter->GetActorLocation();
            const FVector LaunchDirection = (AimLocation - SocketLocation).GetSafeNormal();
            if (LaunchDirection.IsNearlyZero()) continue;
            
            if (AProjectileBase* Projectile = Character->GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SocketLocation, LaunchDirection.Rotation()))
            {
                Projectile->Box->IgnoreActorWhenMoving(Character, true);
                Projectile->Box->MoveIgnoreActors.AddUnique(Character);
                Projectile->Init(Character, TargetCharacter, Skill, LaunchDirection);
            }
        }
    }

    if (OnFinished.IsBound()) OnFinished.Execute();
}

void USkillTaskSpawnProjectile::Cancel()
{
    
}



