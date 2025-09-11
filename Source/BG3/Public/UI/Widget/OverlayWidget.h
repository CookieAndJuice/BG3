
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayWidget.generated.h"

class UCombatActionPanel;
class UOverlayWidgetController;

UCLASS()
class BG3_API UOverlayWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Combat|UI")
    void SetController(UOverlayWidgetController* InController);

protected:
    virtual void NativeConstruct() override;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UOverlayWidgetController* Controller = nullptr;

    // Optional binding to the combat action panel within this overlay
    UPROPERTY(meta=(BindWidgetOptional), BlueprintReadOnly)
    UCombatActionPanel* ActionPanel = nullptr;
};
