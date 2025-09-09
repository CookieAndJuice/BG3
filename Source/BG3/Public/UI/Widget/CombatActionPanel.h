
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatActionPanel.generated.h"

class UUniformGridPanel;
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
    UPROPERTY(meta=(BindWidgetOptional), BlueprintReadWrite)
    UUniformGridPanel* Panel_Root = nullptr;

    UPROPERTY(EditDefaultsOnly, Category="Combat|UI")
    TSubclassOf<UActionSlotEntry> ActionSlotEntryClass;

    UPROPERTY(EditAnywhere, Category="Combat|UI")
    int32 NumColumns = 6;

private:
    UPROPERTY()
    UCombatActionWidgetController* Controller = nullptr;

    UFUNCTION()
    void RebuildSlots(const TArray<FActionSlotView>& Slots);

    UFUNCTION()
    void OnSkillButtonClicked(int32 SkillID);
};
