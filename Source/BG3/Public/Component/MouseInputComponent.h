#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "MouseInputComponent.generated.h"

class UInputAction;

/**
 * 마우스 입력을 통해 타겟 선택/확정/취소를 담당하는 간단 컴포넌트.
 * - PlayerController에 부착하여 사용(Enhanced Input 바인딩은 PC의 SetupInputComponent에서 넘겨받음)
 * - 스킬 캐스팅이 Targeting 상태일 때만 동작
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BG3_API UMouseInputComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMouseInputComponent();

    /** PlayerController의 EnhancedInputComponent로 입력 바인딩을 위임받아 연결 */
    void BindInput(class UEnhancedInputComponent* EIC);

    /** 좌클릭 등: 커서 아래 액터 1명을 타겟으로 지정(SetTargets) */
    void OnClick(const FInputActionValue& Value);

    /** 확정 키: ConfirmAndExecute 호출 */
    void OnConfirm(const FInputActionValue& Value);

    /** 취소 키: CancelCast 호출 */
    void OnCancel(const FInputActionValue& Value);

public:
    // 입력 액션(에셋이 없으면 바인딩 생략)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> ClickAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> ConfirmAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> CancelAction;

private:
    /** 커서 아래 액터 1개를 트레이스로 구한다(없으면 nullptr) */
    AActor* GetActorUnderCursor() const;

    /** 현재 Subsystem이 Targeting 상태인지 */
    bool IsTargeting() const;
};

