
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterArchetype.generated.h"

class USkillSet;
class USkillDefinition;

UCLASS()
class BG3_API UCharacterArchetype : public UDataAsset
{
	GENERATED_BODY()

public:
	
	// 편의용 표시 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Archetype|Meta")
	FText DisplayName;

	// 기본으로 지급할 스킬 묶음들(여러 개 가능)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Archetype|Skills")
	TArray<TObjectPtr<USkillSet>> SkillSets;

	// 묶음과 별개로 추가 지급할 개별 스킬(옵션)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Archetype|Skills")
	TArray<TObjectPtr<USkillDefinition>> ExtraSkills;

	/** 모든 스킬을 한 번에 모아(중복 제거) 반환하는 헬퍼 */
	void CollectAllSkills(TSet<USkillDefinition*>& OutUnique) const;
};
