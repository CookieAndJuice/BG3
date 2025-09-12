
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/SkillDefinition.h"
#include "SkillBookComponent.generated.h"


// 스킬 런타임 상태
USTRUCT(BlueprintType)
struct FSkillRuntimeState
{
	GENERATED_BODY()

	// 다시 사용 가능해지까지 남은 라운드 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CooldownRemainingRounds = 0;

	// 충전형 스킬일 때 남은 충전 횟수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentCharges = -1;

	// 이 스킬을 마지막으로 성공적으로 사용한 라운드 인덱스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 LastUsedRound = -1;

	// 이번 턴에 이 스킬을 몇 번 썼는지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TimesUsedThisTurn = 0;

	// 이 스킬이 시전 예약 상태인지 확인
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bReserved = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCooldownChanged, const USkillDefinition*, Skill, int32, NewRounds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUsabilityChanged, const USkillDefinition*, Skill, bool, bUsable);



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BG3_API USkillBookComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillBookComponent();
	
	/** 보유 스킬 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SkillBook")
	TArray<TObjectPtr<USkillDefinition>> Skills;

	/** 스킬별 런타임 상태 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SkillBook")
	TMap<TObjectPtr<USkillDefinition>, FSkillRuntimeState> Runtime;

	/** 쿨다운/사용가능 이벤트 */
	UPROPERTY(BlueprintAssignable)
	FOnCooldownChanged OnCooldownChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnUsabilityChanged OnUsabilityChanged;

public:
	/** 스킬 추가/제거 */
	UFUNCTION(BlueprintCallable, Category="SkillBook")
	void AddSkill(USkillDefinition* Def, bool bInitCharges = false, int32 InitialCharges = -1);

	UFUNCTION(BlueprintCallable, Category="SkillBook")
	void RemoveSkill(USkillDefinition* Def);

	/** 사용 가능 여부 판정 */
	UFUNCTION(BlueprintCallable, Category="SkillBook")
	void GetUsability(USkillDefinition* Def, bool& bOutUsable) const;

	/** 예약 → 커밋/환불 */
	UFUNCTION(BlueprintCallable, Category="SkillBook")
	bool ReserveUse(USkillDefinition* Def);

	UFUNCTION(BlueprintCallable, Category="SkillBook")
	void CommitUse(USkillDefinition* Def, int32 CurrentRound);

    UFUNCTION(BlueprintCallable, Category="SkillBook")
    void CancelReservation(USkillDefinition* Def);

	/** 턴 훅: 소유자 턴 시작에 호출(쿨다운 -1, 턴 카운터 리셋) */
    UFUNCTION(BlueprintCallable, Category="SkillBook")
    void OnOwnerTurnStart();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category="SkillBook")
    int32 GetCooldownRemaining(USkillDefinition* Def) const;

protected:
	virtual void BeginPlay() override;

private:
	bool HasSkill(const USkillDefinition* Def) const;
	bool QueryCanSpendSlot(EActionCost Cost) const;
	void SpendSlot(EActionCost Cost) const;
	void RefundSlot(EActionCost Cost) const;
	
};
