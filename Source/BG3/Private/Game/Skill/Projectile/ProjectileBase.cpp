
#include "Game/Skill/Projectile/ProjectileBase.h"

#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Data/SkillDefinition.h"
#include "Game/BG3GameMode.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Manager/BG3DiceManager.h"


AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Box);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Box);

	Trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	Trail->SetupAttachment(GetRootComponent());

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(GetRootComponent());

	
}

void AProjectileBase::Init(ABaseCharacter* Caster, ABaseCharacter* Target, const USkillDefinition* Skill)
{
	if (!Caster || !Skill) return;

	ProjectileMovement->InitialSpeed = 400.f;

	int32 DiceNum = FMath::Max(1, Skill->Damage.Dice.Num);
	int32 DiceSides = FMath::Max(1, Skill->Damage.Dice.Sides);
	Damage = DiceNum * DiceSides;

	if (ABG3GameMode* GM = GetWorld() ? GetWorld()->GetAuthGameMode<ABG3GameMode>() : nullptr)
	{
		if (GM->Dice)
		{
			Damage = GM->Dice->RollDice(DiceNum, DiceSides);
		}
	}

	FVector TargetLocation = Target->GetActorLocation();
	FVector StartLocation = GetActorLocation();
	FVector Direction = (TargetLocation - StartLocation).GetSafeNormal();

	SetOwner(Caster);
	SetActorRotation(Direction.Rotation());

	

	LaunchInDirection(Direction);
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnBoxComponentHit);
	
}

void AProjectileBase::OnImpactFX(const FHitResult& Hit)
{
}

void AProjectileBase::LaunchInDirection(const FVector& Dir)
{
	if (!ProjectileMovement) return;

	FVector Direction = Dir.GetSafeNormal();

	float Speed = ProjectileMovement->InitialSpeed;

	ProjectileMovement->Velocity = Direction * Speed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->UpdateComponentVelocity();
	ProjectileMovement->Activate(true);
	
	
}

void AProjectileBase::OnBoxComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	PRINTLOG(TEXT("Projectile Overlapped To Target. Damage : %d"), Damage);

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, [this]()
	{
		Destroy();
	}, 1.f, false);
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}




