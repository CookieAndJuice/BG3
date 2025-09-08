
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatActionPanel.generated.h"

class UPanelWidget;
class UActionSlotEntry;
class UCombatActionWidgetController;
struct FActionSlotView;

UCLASS()
class BG3_API UCombatActionPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Combat|UI")
    void SetController(UCombatActionWidgetController* InController);

protected:
    UPROPERTY(meta=(BindWidgetOptional))
    UPanelWidget* Panel_Root = nullptr;

    UPROPERTY(EditDefaultsOnly, Category="Combat|UI")
    TSubclassOf<UActionSlotEntry> ActionSlotEntryClass;

private:
    UPROPERTY()
    UCombatActionWidgetController* Controller = nullptr;

    UFUNCTION()
    void RebuildSlots(const TArray<FActionSlotView>& Slots);
};
