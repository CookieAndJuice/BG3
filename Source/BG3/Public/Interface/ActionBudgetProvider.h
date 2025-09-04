
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionBudgetProvider.generated.h"

enum EActionCost;

UINTERFACE(MinimalAPI)
class UActionBudgetProvider : public UInterface
{
	GENERATED_BODY()
};


class BG3_API IActionBudgetProvider
{
	GENERATED_BODY()

public:

	// 현재 턴의 예산에서 이 비용을 쓸 수 있나?
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanSpendActionSlot(EActionCost Cost) const;

	// 실제로 소비
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpendActionSlot(EActionCost Cost);

	// 취소/실패 시 환불
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RefundActionSlot(EActionCost Cost);
};
