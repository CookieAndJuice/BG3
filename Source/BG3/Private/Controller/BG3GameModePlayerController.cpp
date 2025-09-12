
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
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "Game/BG3GameMode.h"

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
	//Possess(PossessedCharacter);
	//PossessedCharacter->EnableInput(this);

	// Initialize Camera Setting
	InitializeCamera();
	Possess(BG3Camera);
	BG3Camera->EnableInput(this);
	
	// Create Combat Action Panel
	OverlayWidget = CreateWidget<UOverlayWidget>(this, OverlayWidgetClass);
	OverlayWidget->AddToViewport();
	UOverlayWidgetController* WC = NewObject<UOverlayWidgetController>();
	if (WC)
	{
		if (ABaseCharacter* PCharacter = Cast<ABaseCharacter>(GMSubsystem->GetCurrentPawn()))
		{
			WC->Initialize(PCharacter);
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
}

void ABG3GameModePlayerController::OnMoveCamera(const FInputActionValue& value)
{
	FVector2D inputVec = value.Get<FVector2D>();
	BG3Camera->FreeCamera(inputVec);}

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

	// Possess New Character
	PossessedCharacter->DisableInput(this);

	Possess(NewCharacter);
	PossessedCharacter = NewCharacter;
	PossessedCharacter->EnableInput(this);

	// Switch Camera Target
	// Change UI Skill Info
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
	FVector location = PossessedCharacter->GetActorLocation();
	BG3Camera->SetActorLocation(location);
	BG3Camera->SetFreeCameraMode(true);
}

