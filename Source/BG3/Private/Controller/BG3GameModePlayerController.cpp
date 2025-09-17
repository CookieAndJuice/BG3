
#include "Controller/BG3GameModePlayerController.h"

#include "BG3/BG3.h"
#include "Kismet/GameplayStatics.h"
#include "BG3/Public/Actor/BG3GameCamera.h"
#include "Blueprint/UserWidget.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/Widget/CombatActionPanel.h"
#include "UI/WidgetController/CombatActionWidgetController.h"
#include "Game/BG3GameManageSubsystem.h"
#include "UI/Widget/OverlayWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EnhancedInputSubsystems.h"
#include "BG3/BG3.h"
#include "Character/BG3EnemyCharacter.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "Game/BG3GameMode.h"
#include "Component/MouseInputComponent.h"
#include "Component/SkillBookComponent.h"
#include "UI/Widget/TurnEndWidget.h"
#include "Game/SkillExecutionSubsystem.h"

ABG3GameModePlayerController::ABG3GameModePlayerController()
{
	ConstructorHelpers::FObjectFinder<UInputMappingContext> cameraIMCRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprints/Input/IMC_Camera.IMC_Camera'"));
	if (cameraIMCRef.Succeeded())
	{
		PlayerIMC = cameraIMCRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> cameraMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/IA_CameraMove.IA_CameraMove'"));
	if (cameraMoveRef.Succeeded())
	{
		CameraMoveAction = cameraMoveRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> cameraZoomRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/IA_CameraZoom.IA_CameraZoom'"));
	if (cameraZoomRef.Succeeded())
	{
		CameraZoomAction = cameraZoomRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> cameraRotateRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/IA_CameraRotate.IA_CameraRotate'"));
	if (cameraRotateRef.Succeeded())
	{
		CameraRotateAction = cameraRotateRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> LMBClickRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/IA_LMBClick.IA_LMBClick'"));
	if (LMBClickRef.Succeeded())
	{
		LMBClickAction = LMBClickRef.Object;
	}

    // 마우스 입력 컴포넌트 생성(입력 바인딩은 SetupInputComponent에서 수행)
    MouseInput = CreateDefaultSubobject<UMouseInputComponent>(TEXT("MouseInputComponent"));
}

void ABG3GameModePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Initialize Input
	auto pc = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()); 
	if (subsys)
	{
		subsys->AddMappingContext(PlayerIMC, 0);
	}
	
	// Initialize Character
	GMSubsystem = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>();
	PossessedCharacter = GMSubsystem->GetCurrentPawn();
	BG3Camera = GMSubsystem->BG3Camera;
	
	// Create Combat Action Panel
	OverlayWidget = CreateWidget<UOverlayWidget>(this, OverlayWidgetClass);
	OverlayWidget->AddToViewport();
	UOverlayWidgetController* WC = NewObject<UOverlayWidgetController>(GetWorld());
	if (WC)
	{
		if (ABaseCharacter* PCharacter = Cast<ABaseCharacter>(GMSubsystem->GetCurrentPawn()))
		{
			WC->Initialize(PCharacter, this);
		}
		OverlayWidget->SetController(WC);
	}
	
	SetShowMouseCursor(true);
}

void ABG3GameModePlayerController::UseSkill(int32 SkillID)
{
	PRINTLOG(TEXT("UseSkill"));
}

void ABG3GameModePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABG3GameModePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EIC->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &ABG3GameModePlayerController::OnMoveCamera);
        EIC->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &ABG3GameModePlayerController::OnZoomCamera);
        EIC->BindAction(CameraRotateAction, ETriggerEvent::Triggered, this, &ABG3GameModePlayerController::OnRotateCamera);
		EIC->BindAction(LMBClickAction, ETriggerEvent::Started, this, &ABG3GameModePlayerController::OnLMBClick);
		EIC->BindAction(LMBClickAction, ETriggerEvent::Completed, this, &ABG3GameModePlayerController::OnLMBClick);

        // 마우스 입력(타겟 선택/확정/취소) 바인딩 연결
        if (MouseInput)
        {
            MouseInput->BindInput(EIC);
        }
    }
}

void ABG3GameModePlayerController::OnLMBClick(const FInputActionValue& value)
{
	if (auto* PC = GetWorld()->GetFirstPlayerController())
	{
		bool bIsClicked = value.Get<bool>();
		if (bIsClicked)
			PC->CurrentMouseCursor = EMouseCursor::GrabHand;
		else
			PC->CurrentMouseCursor = EMouseCursor::Default;
	}

	// SkillExecutionSubsystem의 CastState가 ECastState::Targeting 일 때만 동작 (스킬 타겟팅 중일 때만) 
	MouseInput->OnClick(value);
}

void ABG3GameModePlayerController::OnMoveCamera(const FInputActionValue& value)
{
	
	FVector2D inputVec = value.Get<FVector2D>();
	BG3Camera->FreeCamera(inputVec);
}

void ABG3GameModePlayerController::OnZoomCamera(const FInputActionValue& value)
{
	float input =  value.Get<float>();
	BG3Camera->Zoom(input);
}

void ABG3GameModePlayerController::OnRotateCamera(const FInputActionValue& value)
{
	float input = value.Get<float>();
	BG3Camera->RotateCamera(input);
}

void ABG3GameModePlayerController::SwitchToPawn(ABaseCharacter* NewCharacter)
{
    if (!NewCharacter) return;

    // Clear any lingering cast when switching turns/characters
    if (UWorld* World = GetWorld())
    {
        if (USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
        {
            if (SES->IsBusy())
            {
                PRINTLOG(TEXT("[SwitchToPawn] Cancel lingering cast"));
                SES->CancelCast();
            }
        }
    }

    //Possess(NewCharacter);
    PossessedCharacter = NewCharacter;

    // Switch Camera Target
    FVector location = PossessedCharacter->GetActorLocation();
    BG3Camera->FocusCamera(location);

    // Reset Actions Budget 
    if (NewCharacter->GetClass()->ImplementsInterface(UActionBudgetProvider::StaticClass()))
    {
        PRINTLOG(TEXT("[SwitchToPawn] BeginTurnReset for %s"), *NewCharacter->GetName());
        IActionBudgetProvider::Execute_BeginTurnReset(NewCharacter);
        bool bA = IActionBudgetProvider::Execute_CanSpendActionSlot(NewCharacter, EActionCost::Action);
        bool bB = IActionBudgetProvider::Execute_CanSpendActionSlot(NewCharacter, EActionCost::Bonus);
        PRINTLOG(TEXT("[SwitchToPawn] After Reset CanSpend: Action=%d Bonus=%d"), bA, bB);
    }	

    // Reset Turn Variables
    PRINTLOG(TEXT("[SwitchToPawn] Call OnOwnerTurnStart"));
    NewCharacter->SkillBook->OnOwnerTurnStart(); 
	
	if (auto* enemy = Cast<ABG3EnemyCharacter>(PossessedCharacter))
	{
		enemy->SetMyTurn();
	}
	
	CurrentCharacterChanged.ExecuteIfBound(NewCharacter);
}


