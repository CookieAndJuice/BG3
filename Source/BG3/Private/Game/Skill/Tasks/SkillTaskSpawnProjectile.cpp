#include "Game/Skill/Tasks/SkillTaskSpawnProjectile.h"

#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Data/SkillDefinition.h"
#include "Engine/World.h"
#include "Game/Skill/Projectile/ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

void USkillTaskSpawnProjectile::Start(UObject* WorldContext, AActor* Caster, const USkillDefinition* Skill, const TArray<AActor*>& Targets)
{
    if (!WorldContext || !Caster)
    {
        if (OnFailed.IsBound()) OnFailed.Execute(TEXT("InvalidContext"));
        return;
    }

    PRINTLOG(TEXT("[Task] SpawnProjectile (skeleton) by %s"), *Caster->GetName());

    TSubclassOf<AProjectileBase> ProjectileClass = Skill->Damage.ProjectileClass;
    ABaseCharacter* Character = Cast<ABaseCharacter>(Caster);
    if (!Character) return;

    FName SpawnSocketName = Skill->Damage.SpawnSocketName;
    FVector SocketLocation = Character->GetMesh()->GetSocketLocation(SpawnSocketName);
    
    //FRotator SocketRotation = Character->GetMesh()->GetSocketRotation(SpawnSocketName);

    TArray<ABaseCharacter*> ValidTargets;
    ValidTargets.Reserve(Targets.Num());
    for (AActor* Target : Targets)
    {
        if (ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(Target))
        {
            ValidTargets.Add(TargetCharacter);
        }
    }

    if (Targets.Num() > 0)
    {
        for (ABaseCharacter* TargetCharacter : ValidTargets)
        {
            FVector Dir = (TargetCharacter->GetMesh()->GetSocketLocation(FName("Spine")) - SocketLocation).GetSafeNormal();
            FRotator Rot = Dir.Rotation();
            AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SocketLocation, Rot);
            Projectile->ProjectileMovement->Velocity = Dir * 500.f;
            Projectile->Init(Character, TargetCharacter, Skill);
        }
    }
    
    
    
    if (OnFinished.IsBound()) OnFinished.Execute();
}

void USkillTaskSpawnProjectile::Cancel()
{
    
}
