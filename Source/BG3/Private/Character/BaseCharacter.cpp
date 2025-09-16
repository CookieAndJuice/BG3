
#include "Character/BaseCharacter.h"
#include "Data/SkillSet.h"
#include "BG3/BG3.h"
#include "UObject/ConstructorHelpers.h"
#include "Data/SkillDefinition.h"
#include "Component/SkillBookComponent.h"
#include "Component/CharacterStatsComponent.h"
#include "FSM/ActionState.h"
#include "FSM/FSMComponent.h"
#include "FSM/IdleState.h"
#include "FSM/MoveState.h"

ABaseCharacter::ABaseCharacter()
{

    PrimaryActorTick.bCanEverTick = false;

    
    // Set DefaultSkills
    static ConstructorHelpers::FObjectFinder<USkillSet> TempSkillSet(TEXT("/Game/Blueprints/Data/DA_DefaultSkills.DA_DefaultSkills"));
    
	if (TempSkillSet.Succeeded())
	{
		DefaultSkills = TempSkillSet.Object;
    }
    
    // Ensure SkillBook component exists for C++ characters
    SkillBook = CreateDefaultSubobject<USkillBookComponent>(TEXT("SkillBook"));

    // Basic stats (HP/MP)
    Stats = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("CharacterStats"));
}

void ABaseCharacter::BeginTurnReset_Implementation()
{
    CurrentActions = 1;
    CurrentBonusActions = 1;
    CurrentReactions = 1;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GrantSkills();
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

    if (!ClassSkills) return;

    for (USkillDefinition* Def : ClassSkills->Skills)
    {
        if (Def)
        {
            SkillBook->AddSkill(Def);
        }
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
        CurrentActions--;
        return;
    case EActionCost::Bonus:
        CurrentBonusActions--;
        return;
    case EActionCost::Reaction:
        CurrentReactions--;
        return;
    }

    PRINTLOG(TEXT("After Spend CurrentActionslot : %d"), CurrentActions);
}

bool ABaseCharacter::CanSpendActionSlot_Implementation(EActionCost Cost) const
{
    switch (Cost)
    {
    case EActionCost::Action:
        return CurrentActions > 0;
    case EActionCost::Bonus:
        return CurrentBonusActions > 0;
    case EActionCost::Reaction:
        return CurrentReactions > 0;
    default:
        return false;
    }
}

void ABaseCharacter::RefundActionSlot_Implementation(EActionCost Cost)
{
    switch (Cost)
    {
    case EActionCost::Action:
        CurrentActions++;
        return;
    case EActionCost::Bonus:
        CurrentBonusActions++;
        return;
    case EActionCost::Reaction:
        CurrentReactions++;
        return;
    }

    PRINTLOG(TEXT("After RefundActionSlot : %d"), CurrentActions);
}