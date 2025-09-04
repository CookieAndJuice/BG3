#include "Data/CharacterArchetype.h"
#include "Data/SkillSet.h"

void UCharacterArchetype::CollectAllSkills(TSet<USkillDefinition*>& OutUnique) const
{
	OutUnique.Reserve(OutUnique.Num() + ExtraSkills.Num() + 8);

	// 1) 묶음에 들어있는 스킬들
	for (const USkillSet* Set : SkillSets)
	{
		if (!Set) continue;
		for (USkillDefinition* Def : Set->Skills)
		{
			if (Def) { OutUnique.Add(Def); }
		}
	}

	// 2) 추가 개별 스킬들
	for (USkillDefinition* Def : ExtraSkills)
	{
		if (Def) { OutUnique.Add(Def); }
	}
}
