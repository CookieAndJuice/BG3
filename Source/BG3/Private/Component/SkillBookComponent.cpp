
#include "Component/SkillBookComponent.h"

#include "Interface/ActionBudgetProvider.h"

USkillBookComponent::USkillBookComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void USkillBookComponent::AddSkill(USkillDefinition* Def, bool bInitCharges, int32 InitialCharges)
{
	if (!Def) return;
	if (!Runtime.Contains(Def))
	{
		Runtime.Add(Def, FSkillRuntimeState{});
	}
	if (!Skills.Contains(Def))
	{
		Skills.Add(Def);
	}
	if (bInitCharges)
	{
		if (FSkillRuntimeState* State = Runtime.Find(Def))
		{
			State->CurrentCharges = InitialCharges;
		}
	}
	OnUsabilityChanged.Broadcast(Def, true);
}

void USkillBookComponent::RemoveSkill(USkillDefinition* Def)
{
	if (!Def) return;
	Runtime.Remove(Def);
	Skills.Remove(Def);
	OnUsabilityChanged.Broadcast(Def, false);
}

void USkillBookComponent::GetUsability(USkillDefinition* Def, bool& bOutUsable) const
{
	bOutUsable = false;

	if (!Def || !HasSkill(Def))
	{
		return;
	}

	const FSkillRuntimeState* State = Runtime.Find(Def);
	if (!State)
	{
		return;
	}

	if (State->bReserved)
	{
		return;
	}

	if (State->CooldownRemainingRounds > 0)
	{
		return;
	}

	if (State->CurrentCharges == 0)
	{
		return;
	}

	// 행동 슬롯 검사 (Free는 스킵)
	if (!QueryCanSpendSlot(Def->Cost.ActionCost))
	{
		return;
	}

	bOutUsable = true;
}

bool USkillBookComponent::ReserveUse(USkillDefinition* Def)
{
	bool bOk = false;
	GetUsability(Def, bOk);
	if (!bOk)
	{
		OnUsabilityChanged.Broadcast(Def, false);
		return false;
	}

	FSkillRuntimeState* State = Runtime.Find(Def);
	if (!State) return false;

	// 슬롯 선점
	if (!QueryCanSpendSlot(Def->Cost.ActionCost))
	{
		OnUsabilityChanged.Broadcast(Def, false);
		return false;
	}
	SpendSlot(Def->Cost.ActionCost);

	State->bReserved = true;
	OnUsabilityChanged.Broadcast(Def, true);
	return true;
}

void USkillBookComponent::CommitUse(USkillDefinition* Def, int32 CurrentRound)
{
	if (!Def) return;
	FSkillRuntimeState* State = Runtime.Find(Def);
	if (!State) return;

	// 쿨다운 시작
	State->CooldownRemainingRounds = Def->Cost.CooldownRounds;
	State->LastUsedRound = CurrentRound;
	State->TimesUsedThisTurn += 1;
	State->bReserved = false;

	// 충전제라면 차감
	if (State->CurrentCharges > 0)
	{
		State->CurrentCharges -= 1;
	}

	OnCooldownChanged.Broadcast(Def, State->CooldownRemainingRounds);
	OnUsabilityChanged.Broadcast(Def, false);
}

void USkillBookComponent::CancelReservation(USkillDefinition* Def)
{
	if (!Def) return;
	FSkillRuntimeState* State = Runtime.Find(Def);
	if (!State) return;
	if (!State->bReserved) return;

	// 슬롯 환불
	RefundSlot(Def->Cost.ActionCost);

	State->bReserved = false;

	// 상태가 바뀌었으니 UI에 알림
	bool bDummy; 
	GetUsability(Def, bDummy);
	OnUsabilityChanged.Broadcast(Def, bDummy);
}

void USkillBookComponent::OnOwnerTurnStart()
{
	// 턴 시작: 쿨다운 -1, 턴 카운터 리셋
	for (auto& Pair : Runtime)
	{
		USkillDefinition* Def = Pair.Key;
		FSkillRuntimeState& State = Pair.Value;

		if (State.CooldownRemainingRounds > 0)
		{
			State.CooldownRemainingRounds = FMath::Max(0, State.CooldownRemainingRounds - 1);
			OnCooldownChanged.Broadcast(Def, State.CooldownRemainingRounds);
		}
		State.TimesUsedThisTurn = 0;

		// 예약이 남아있으면 안전하게 해제(엣지 케이스)
		if (State.bReserved)
		{
			// 예약 해제 시 슬롯 환불
			RefundSlot(Def->Cost.ActionCost);
			State.bReserved = false;
		}

		// 사용 가능 상태 재통지(선택)
		bool bDummy;
		GetUsability(Def, bDummy);
		OnUsabilityChanged.Broadcast(Def, bDummy);
	}
}

bool USkillBookComponent::HasSkill(const USkillDefinition* Def) const
{
	return Def && Skills.Contains(const_cast<USkillDefinition*>(Def));
}

bool USkillBookComponent::QueryCanSpendSlot(EActionCost Cost) const
{
	const AActor* OwnerActor = GetOwner();
	if (OwnerActor && OwnerActor->GetClass()->ImplementsInterface(UActionBudgetProvider::StaticClass()))
	{
		return IActionBudgetProvider::Execute_CanSpendActionSlot(OwnerActor, Cost);
	}
	// 인터페이스 없으면 슬롯 체크 스킵
	return true;
}

void USkillBookComponent::SpendSlot(EActionCost Cost) const
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor && OwnerActor->GetClass()->ImplementsInterface(UActionBudgetProvider::StaticClass()))
	{
		IActionBudgetProvider::Execute_SpendActionSlot(OwnerActor, Cost);
	}
}

void USkillBookComponent::RefundSlot(EActionCost Cost) const
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor && OwnerActor->GetClass()->ImplementsInterface(UActionBudgetProvider::StaticClass()))
	{
		IActionBudgetProvider::Execute_RefundActionSlot(OwnerActor, Cost);
	}
}

void USkillBookComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 런타임 맵 초기화
	for (USkillDefinition* Def : Skills)
	{
		if (Def && !Runtime.Contains(Def))
		{
			Runtime.Add(Def, FSkillRuntimeState{});
		}
	}
}

