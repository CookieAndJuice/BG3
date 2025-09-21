#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnEndWidget.generated.h"

class UOverlayWidgetController;
class UButton;

DECLARE_DELEGATE(FOnTurnEndDelegate);

UCLASS()
class BG3_API UTurnEndWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    FOnTurnEndDelegate OnTurnEndDelegate;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
    TObjectPtr<UButton> TurnEndButton;

    UFUNCTION()
    void OnTurnEndButtonHovered();

    UFUNCTION()
    void OnTurnEndButtonPressed();

    UFUNCTION()
    void OnTurnEndButtonReleased();

    UFUNCTION()
    void OnTurnEndButtonClicked();

    void BindController(UOverlayWidgetController* InController);

    UFUNCTION()
    void HandleMoveDistanceChanged(float Remaining, float Max);

    UFUNCTION(BlueprintImplementableEvent, Category="Combat|UI")
    void UpdateMoveDistance(float Remaining, float Max);

private:
    UPROPERTY()
    TObjectPtr<UOverlayWidgetController> Controller;
};
