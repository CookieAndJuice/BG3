
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/SkillDefinition.h"
#include "ActionSlotEntry.generated.h"

class UTexture2D;

enum class EActionCost : uint8;

USTRUCT(BlueprintType)
struct FActionSlotView
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillID = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconBG = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EActionCost ActionCost = EActionCost::Action;  // 예: Action=1, Bonus=0 ...
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsable = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CooldownRemain = 0;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, int32, SkillID);

UCLASS()
class BG3_API UActionSlotEntry : public UUserWidget
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    void Setup(const FActionSlotView& InView);

	UPROPERTY(BlueprintAssignable)
	FOnSlotClicked OnClicked;

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_Use = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	class UImage* Img_Icon = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	class UBorder* Overlay_Disalbed = nullptr;


private:
	
	FActionSlotView View;

	// Keep a reference so GC doesn't collect the texture used in the button style
	UPROPERTY()
	UTexture2D* CachedIconBG = nullptr;

	UFUNCTION()
	void HandleClick();

	
};
