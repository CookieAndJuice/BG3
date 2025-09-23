
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayWidget.generated.h"

enum class EResultState : uint8;
class UCombatActionPanel;
class UOverlayWidgetController;
class UTurnEndWidget;
class UPlayerHPWidget;
class UPlayerPortraitWidget;
class UTurnOrderFrameWidget;
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

    UPlayerHPWidget* GetPlayerHPWidget();
    UTurnOrderFrameWidget* GetTurnOrderFrame();
    UPlayerPortraitWidget* GetPlayerPortraitWidget();

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
    UPlayerHPWidget* PlayerHPWidget = nullptr;

    UPROPERTY(meta=(BindWidgetOptional), BlueprintReadOnly)
    UPlayerPortraitWidget* PlayerPortrait = nullptr;

    UPROPERTY(meta=(BindWidgetOptional), BlueprintReadOnly)
    UTurnOrderFrameWidget* TurnOrderFrame = nullptr;

    UPROPERTY(meta=(BindWidgetOptional), BlueprintReadOnly)
    UTextBlock* Text_Win;

    UPROPERTY(meta=(BindWidgetOptional), BlueprintReadOnly)
    UTextBlock* Text_Lose;
};
