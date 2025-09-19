
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayWidget.generated.h"

class UCombatActionPanel;
class UOverlayWidgetController;
class UTurnEndWidget;
class UTextBlock;

UCLASS()
class BG3_API UOverlayWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Combat|UI")
    void SetController(UOverlayWidgetController* InController);

    UFUNCTION()
    void FadeOutAnimationStart(EResultState result);

    UPROPERTY(meta=(BindWidgetAnim), Transient)
    UWidgetAnimation* FadeOut1;

    UPROPERTY(meta=(BindWidgetAnim), Transient)
    UWidgetAnimation* FadeOut2;

    UPROPERTY(meta=(BindWidgetAnim), Transient)
    UWidgetAnimation* FadeOut3;


protected:
    virtual void NativeConstruct() override;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UOverlayWidgetController* Controller = nullptr;

    // Optional binding to the combat action panel within this overlay
    UPROPERTY(meta=(BindWidgetOptional), BlueprintReadOnly)
    UCombatActionPanel* ActionPanel = nullptr;

    UPROPERTY(meta=(BindWidgetOptional), BlueprintReadOnly)
    UTurnEndWidget* TurnEndWidget = nullptr;

    UPROPERTY(meta=(BindWidgetOptional), BlueprintReadOnly)
    UTextBlock* Text_Win;

    UPROPERTY(meta=(BindWidgetOptional), BlueprintReadOnly)
    UTextBlock* Text_Lose;
};
