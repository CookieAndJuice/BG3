
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
#include "Character/BG3PlayerCharacter.h"
#include "Component/CharacterStatsComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "Game/BG3GameMode.h"
#include "Component/MouseInputComponent.h"
#include "Component/SkillBookComponent.h"
#include "Component/TurnWidgetManagerComponent.h"
#include "Dataflow/DataflowOverlay.h"
#include "UI/Widget/TurnEndWidget.h"
#include "Game/SkillExecutionSubsystem.h"
#include "UI/Widget/PlayerPortraitWidget.h"
#include "UI/Widget/TurnOrderFrameWidget.h"

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
	ConstructorHelpers::FClassFinder<ABG3GameCamera> CameraClassRef(TEXT("'/Game/Blueprints/Actor/BP_GameCamera.BP_GameCamera_C'"));
	if (CameraClassRef.Succeeded())
	{
		BG3CameraClass = CameraClassRef.Class;
	}

    // ���콺 �Է� ������Ʈ ����(�Է� ���ε��� SetupInputComponent���� ����)
    MouseInput = CreateDefaultSubobject<UMouseInputComponent>(TEXT("MouseInputComponent"));
}

void ABG3GameModePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Initialize Input
	auto pc = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()); 
	if (!subsys) return;
	subsys->AddMappingContext(PlayerIMC, 0);
	
	// Initialize Character
	GMSubsystem = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>();
	PossessedCharacter = GMSubsystem->GetCurrentPawn();

	// Initialize Camera Setting
	InitializeCamera();
	Possess(BG3Camera);
	BG3Camera->EnableInput(this);
	
	// Create Overlay Widget
	OverlayWidget = CreateWidget<UOverlayWidget>(this, OverlayWidgetClass);
	OverlayWidget->AddToViewport();
	UOverlayWidgetController* WC = NewObject<UOverlayWidgetController>(GetWorld());
	if (WC)
	{
		if (ABG3PlayerCharacter* PCharacter = Cast<ABG3PlayerCharacter>(GMSubsystem->GetCurrentPawn()))
		{
			WC->Initialize(PCharacter, this);
		}
		else if (ABG3EnemyCharacter* PEnemy = Cast<ABG3EnemyCharacter>(GMSubsystem->GetCurrentPawn()))
		{
			UBG3GameManageSubsystem* Sub = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>();
			ABaseCharacter* Link = Sub->GetPlayerFromID(1);
			WC->Initialize(Link, this);
		}
		
		OverlayWidget->SetController(WC);
	}

	// Add CharacterWigets to TurnOrderFrame
	for (auto pawnData : GMSubsystem->CombatPawns)
	{
		UTurnWidgetManagerComponent* turnWidgetManager = pawnData.TurnCharacter->TurnWidgetManager;
		turnWidgetManager->AddWidgetToArray();
	}
	// Add to Turn Order Widgets to Overlay Widget
	OverlayWidget->GetTurnOrderFrame()->CreateFrameWidget();
	if (Cast<ABG3PlayerCharacter>(PossessedCharacter))
	{
		OverlayWidget->GetPlayerPortraitWidget()->ChangePortrait(PossessedCharacter->GetPortrait());
	}
	
	SetShowMouseCursor(true);
}

void ABG3GameModePlayerController::SpawnCamera()
{
	FRotator spawnRotation = FRotator(0, 0, 0);
	// FVector spawnLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	FVector spawnLocation = GMSubsystem->GetCurrentPawn()->GetActorLocation();
	FTransform spawnTransform(spawnRotation, spawnLocation);

	BG3Camera = GetWorld()->SpawnActor<ABG3GameCamera>(BG3CameraClass, spawnTransform);
}

void ABG3GameModePlayerController::InitializeCamera()
{
	SpawnCamera();
	SetViewTargetWithBlend(BG3Camera);

	// Set Follow Mode
	BG3Camera->FocusCamera(PossessedCharacter);
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

        // ���콺 �Է�(Ÿ�� ����/Ȯ��/���) ���ε� ����
        if (MouseInput)
        {
            MouseInput->BindInput(EIC);
        }
    }
}

void ABG3GameModePlayerController::OnLMBClick(const FInputActionValue& value)
{
	bool bIsClicked = value.Get<bool>();
	
	if (auto* PC = GetWorld()->GetFirstPlayerController())
	{
		if (bIsClicked)
		{
			PRINTLOG(TEXT("bIsClicked true"));
			PC->CurrentMouseCursor = EMouseCursor::GrabHand;
			MouseInput->OnClick(value);	
		}
		else
		{
			PRINTLOG(TEXT("bIsClicked false"));
			PC->CurrentMouseCursor = EMouseCursor::Default;
		}
	}

	// SkillExecutionSubsystem�� CastState�� ECastState::Targeting �� ���� ���� (��ų Ÿ���� ���� ����)
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
    BG3Camera->FocusCamera(PossessedCharacter);
	BG3Camera->CustomZoom(1, BG3Camera->MaxTargetArmLength, BG3Camera->MinPitch);

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
	
    // Reset RemainingMoveDistance
    if (MouseInput)
    {
        MouseInput->SetComponentTickEnabled(false);
        MouseInput->PendingTurnMove.Reset();
    }
    NewCharacter->Stats->ResetRemainingMoveDistance();

	// enemy
	if (auto* enemy = Cast<ABG3EnemyCharacter>(PossessedCharacter))
	{
		ABG3GameMode* GM = Cast<ABG3GameMode>(GetWorld()->GetAuthGameMode());
		if (GM->IsEnemyWin()) return;
		enemy->SetMyTurn();
		return;
	}
	// else
	// {
	PRINTLOG(TEXT("%s"), *PossessedCharacter->GetName());
	OverlayWidget->GetPlayerPortraitWidget()->ChangePortrait(PossessedCharacter->GetPortrait());
	// }
	
	CurrentCharacterChanged.ExecuteIfBound(NewCharacter);
}




