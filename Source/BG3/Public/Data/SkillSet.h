
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillSet.generated.h"

class USkillDefinition;


UCLASS()
class BG3_API USkillSet : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skills")
	TArray<TObjectPtr<USkillDefinition>> Skills;
	
};
