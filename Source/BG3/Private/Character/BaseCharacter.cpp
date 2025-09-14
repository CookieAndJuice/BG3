
#include "Character/BaseCharacter.h"
#include "Data/SkillSet.h"
#include "BG3/BG3.h"
#include "UObject/ConstructorHelpers.h"
#include "Data/SkillDefinition.h"
#include "Component/SkillBookComponent.h"
#include "Component/CharacterStatsComponent.h"
#include "FSM/FSMComponent.h"
#include "FSM/IdleState.h"

ABaseCharacter::ABaseCharacter()
{

    PrimaryActorTick.bCanEverTick = false;
    
    static ConstructorHelpers::FObjectFinder<USkillSet> TempSkillSet(TEXT("/Game/Blueprints/Data/DA_DefaultSkills.DA_DefaultSkills"));

    
	if (TempSkillSet.Succeeded())
	{
		DefaultSkills = TempSkillSet.Object;
    }
    
    // Ensure SkillBook component exists for C++ characters
    SkillBook = CreateDefaultSubobject<USkillBookComponent>(TEXT("SkillBook"));

    // Basic stats (HP/MP)
    Stats = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("CharacterStats"));

    FSMComp = CreateDefaultSubobject<UFSMComponent>(TEXT("FSMComp"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GrantSkills();
    BuildFSM();
}

void ABaseCharacter::GrantSkills()
{
    if (!SkillBook)
    {
        SkillBook = FindComponentByClass<USkillBookComponent>();
    }

    if (!SkillBook || !DefaultSkills)
    {
        return;
    }

    for (USkillDefinition* Def : DefaultSkills->Skills)
    {
        if (Def)
        {
            SkillBook->AddSkill(Def);
        }
    }
}

void ABaseCharacter::BuildFSM()
{
    // Init FSM StateMap
    if (FSMComp)
    {
        FSMComp->StateClasses.Add(ECharacterState::Idle, UIdleState::StaticClass());
        FSMComp->ChangeState(*this, ECharacterState::Idle);
    }
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::SpendActionSlot_Implementation(EActionCost Cost)
{
    switch (Cost)
    {
    case EActionCost::Action:
        Actions--;
        return;
    case EActionCost::Bonus:
        BonusActions--;
        return;
    case EActionCost::Reaction:
        Reactions--;
        return;
    }

    PRINTLOG(TEXT("After SpendActionSlot : %d"), Actions);
}

bool ABaseCharacter::CanSpendActionSlot_Implementation(EActionCost Cost) const
{
    switch (Cost)
    {
    case EActionCost::Action:
        return Actions > 0;
    case EActionCost::Bonus:
        return BonusActions > 0;
    case EActionCost::Reaction:
        return Reactions > 0;
    default:
        return false;
    }
}

void ABaseCharacter::RefundActionSlot_Implementation(EActionCost Cost)
{
    switch (Cost)
    {
    case EActionCost::Action:
        Actions++;
        return;
    case EActionCost::Bonus:
        BonusActions++;
        return;
    case EActionCost::Reaction:
        Reactions++;
        return;
    }

    PRINTLOG(TEXT("After RefundActionSlot : %d"), Actions);
}