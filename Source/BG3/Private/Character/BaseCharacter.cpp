
#include "Character/BaseCharacter.h"
#include "Data/SkillSet.h"
#include "BG3/BG3.h"
#include "UObject/ConstructorHelpers.h"
#include "Data/SkillDefinition.h"
#include "Component/SkillBookComponent.h"
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
