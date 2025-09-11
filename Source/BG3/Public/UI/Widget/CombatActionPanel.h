
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/ActionSlotEntry.h"
#include "CombatActionPanel.generated.h"

class UUniformGridPanel;
class UPanelWidget;
class UActionSlotEntry;
class UOverlayWidgetController;
struct FActionSlotView;
class UGridPanel;



UCLASS()
class BG3_API UCombatActionPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Combat|UI")
    void SetController(UOverlayWidgetController* InController);


protected:
    UPROPERTY(meta=(BindWidgetOptional), BlueprintReadWrite)
    UGridPanel* Panel_Root = nullptr;

    UPROPERTY(EditDefaultsOnly, Category="Combat|UI")
    TSubclassOf<UActionSlotEntry> ActionSlotEntryClass;

    UPROPERTY(EditAnywhere, Category="Combat|UI")
    int32 NumRow = 2;

    UPROPERTY()
    UOverlayWidgetController* Controller = nullptr;

    UFUNCTION()
    void RebuildSlots(const TArray<FActionSlotView>& Slots);

    UFUNCTION()
    void OnSkillButtonClicked(int32 SkillID);

private:
    
};
