
#include "Game/Skill/Projectile/ProjectileBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Actor/BG3GameCamera.h"
#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Controller/BG3GameModePlayerController.h"
#include "Data/SkillDefinition.h"
#include "Game/BG3GameMode.h"
#include "Game/BG3GameState.h"
#include "Game/SkillExecutionSubsystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Manager/BG3DiceManager.h"


AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Box);
	ProjectileMovement->SetActive(false);

	Trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	Trail->SetupAttachment(GetRootComponent());

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(GetRootComponent());

	
}

void AProjectileBase::Init(ABaseCharacter* Caster, ABaseCharacter* Target, const USkillDefinition* Skill, const FVector& LaunchDirection)
{
	if (!Caster || !Skill) return;

	ProjectileMovement->InitialSpeed = FMath::Max(ProjectileMovement->InitialSpeed, 400.f);
	ProjectileMovement->MaxSpeed = FMath::Max(ProjectileMovement->MaxSpeed, ProjectileMovement->InitialSpeed);
	
	int32 DiceNum = FMath::Max(1, Skill->Damage.Dice.Num);
	int32 DiceSides = FMath::Max(1, Skill->Damage.Dice.Sides);
	Damage = DiceNum * DiceSides;

	CasterCharacter = Caster;
	TargetCharacter = Target;

	if (ABG3GameMode* GM = GetWorld() ? GetWorld()->GetAuthGameMode<ABG3GameMode>() : nullptr)
	{
		if (GM->Dice)
		{
			Damage = GM->Dice->RollDice(DiceNum, DiceSides);
		}
	}

	SetOwner(Caster);

	const FVector Direction = LaunchDirection.GetSafeNormal();
	if (Direction.IsNearlyZero()) return;

	//SetActorRotation(Direction.Rotation());
	LaunchInDirection(Direction);
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	//Box->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnBoxComponentHit);
}

void AProjectileBase::OnImpactFX(const FHitResult& Hit)
{
}

void AProjectileBase::LaunchInDirection(const FVector& Dir)
{
	if (!ProjectileMovement) return;

	const FVector Direction = Dir.GetSafeNormal();
	//if (Direction.IsNearlyZero()) return;

	ProjectileMovement->Velocity = Direction * 450.f;
	//ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->Activate(true);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + ProjectileMovement->Velocity, FColor::Red);

	ProjectileMovement->UpdateComponentVelocity();
}

void AProjectileBase::OnBoxComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	PRINTLOG(TEXT("Projectile Overlapped To Target. Damage : %d"), Damage);

	//FinalizeCast()
	
	
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, [this]()
	{
		Destroy();
	}, 1.f, false);
	
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DistanceToPlayer = (TargetCharacter->GetActorLocation() - GetActorLocation()).Size();

	if (DistanceToPlayer < 5.f)
	{
		FHitResult OutHit;
		FVector Start = GetActorLocation();
		FVector End = Start + GetActorForwardVector() * 50.f;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		
		if (GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECollisionChannel::ECC_Pawn,
			FCollisionShape::MakeSphere(50.f), Params))
		{
			if (TargetCharacter != OutHit.GetActor()) return;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, OutHit.ImpactPoint);
		}
		FinalizeCast(CasterCharacter, TargetCharacter);
	}
}

void AProjectileBase::FinalizeCast(ABaseCharacter* Caster, ABaseCharacter* Target)
{
	if (Caster)
	{
		if (UWorld* World = Caster->GetWorld())
		{
			if (USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
			{
				TArray<AActor*> Targets;
				Targets.Add(TargetCharacter);
				Target->SetIsHit(true);

				if (ABG3GameState* GState = World->GetGameState<ABG3GameState>())
				{
					SES->FinalizeCastAfterExecutor(Targets, GState->GetCurrentRound());	
				}
			}
		}
	}

	Cast<ABG3GameModePlayerController>(GetWorld()->GetFirstPlayerController())->BG3Camera->StopAttackCamera();
	Destroy();
}





