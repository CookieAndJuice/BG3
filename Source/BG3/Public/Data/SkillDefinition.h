
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SkillDefinition.generated.h"

class UTexture2D;

UENUM(BlueprintType)
enum class ETargetingMode : uint8
{
	None,
	Actor,
	Point,
	Area,
	Self
};

UENUM(BlueprintType)
enum class ESkillKind : uint8
{
	Melee,
	Ranged,
	Spell,
	NonCombat
};

UENUM(BlueprintType)
enum class ESkillShape : uint8
{
	Single,
	Circle
};

UENUM(BlueprintType)
enum class EResolutionMode : uint8
{
	ToHit,
	Save,
	AutoHit
};

UENUM(BlueprintType)
enum class EActionCost : uint8
{
	Action,
	Bonus,
	Reaction
};

USTRUCT(BlueprintType)
struct FDice
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Num = 1;   // N
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Sides = 6; // dM
};



USTRUCT(BlueprintType)
struct FSkillMetaData
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Meta")
	int ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Meta")
	FName DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Meta")
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Meta")
	TSoftObjectPtr<UTexture2D> IconBG = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Meta")
	FGameplayTagContainer Tags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Rules")
	ESkillKind SkillKind = ESkillKind::NonCombat;
};

USTRUCT(BlueprintType)
struct FSkillCost
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EActionCost ActionCost = EActionCost::Action;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CooldownRounds = 0;
};

USTRUCT(BlueprintType)
struct FTargeting
{
	GENERATED_BODY();

	// 타게팅 시 모드
	// ex] 스킬 선택 후 타게팅 중일 때 액터 선택하여 타겟 지정할지, 목표 지점을 타겟 위치로 설정할지 결정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETargetingMode TargetingMode;

	// 자기 자신을 타겟으로 설정할 수 있는지 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bAllowSelfTarget = false;

	// 한 번 시전에 총 몇 발/몇 타인지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 HitCountPerCast = 1;

	// 서로 다른 대상을 한 번에 최대 몇 명까지 고를 수 있는지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxTargets;
	
	// 한 대상에게 여러 발 몰아주기 허용?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bAllowMultipleHitsPerTarget;

	// 7. 사거리(m). 근접은 1.5~2.0m 등
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RangeMeters = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESkillShape Shape = ESkillShape::Single;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Radius = 0.f;
	
};

USTRUCT(BlueprintType)
struct FDamagePackage
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DamageType; // 예: Damage.Physical / Damage.Fire

	// NdM
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDice Dice;
};




UCLASS()
class BG3_API USkillDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
	FSkillMetaData Meta;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rules")
	FSkillCost Cost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rules")
	FTargeting Targeting; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rules")
	EResolutionMode Resolution;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rules")
	FDamagePackage Damage;

	
};
