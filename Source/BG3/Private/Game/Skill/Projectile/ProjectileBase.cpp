
#include "Game/Skill/Projectile/ProjectileBase.h"

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

	Trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	Trail->SetupAttachment(GetRootComponent());
}

void AProjectileBase::Init(ABaseCharacter* Caster, ABaseCharacter* Target, USkillDefinition* Skill)
{
	if (!Caster || !Target || !Skill) return;

	ABG3GameMode* GM = GetWorld()->GetAuthGameMode<ABG3GameMode>();
	if (!GM) return;


	// 데미지 결정
	int32 DiceNum = Skill->Damage.Dice.Num;
	int32 DiceSides = Skill->Damage.Dice.Sides;
	
	Damage = GM->Dice->RollDice(DiceNum, DiceSides);

	// 방향 결정
	FVector Direction = (Caster->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal();
	
	// 그 방향으로 발사하기
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
}

void AProjectileBase::OnBoxComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

