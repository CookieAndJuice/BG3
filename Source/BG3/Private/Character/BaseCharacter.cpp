
#include "Character/BaseCharacter.h"
#include "Data/SkillSet.h"
#include "BG3/BG3.h"
#include "UObject/ConstructorHelpers.h"
#include "BG3/BG3.h"
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
	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::GrantSkillsFromArchetype()
{
	for (USkillDefinition* Def : DefaultSkills)
	{
		SkillBook->AddSkill(Def);
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

