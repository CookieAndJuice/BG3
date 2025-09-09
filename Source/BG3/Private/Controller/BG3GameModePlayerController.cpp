
#include "Controller/BG3GameModePlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "BG3/Public/Actor/BG3GameCamera.h"
#include "Blueprint/UserWidget.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/Widget/CombatActionPanel.h"
#include "UI/WidgetController/CombatActionWidgetController.h"
#include "Game/BG3GameManageSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"

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

	// Initialize Camera Setting
	InitializeCamera();
	
	// Create Combat Action Panel
	ActionPanel = CreateWidget<UCombatActionPanel>(this, ActionPanelClass);
	ActionPanel->AddToViewport();
	UCombatActionWidgetController* WC = NewObject<UCombatActionWidgetController>();
	if (WC) ActionPanel->SetController(WC);
	ABaseCharacter* PCharacter = Cast<ABaseCharacter>(GetCharacter());
	WC->Initialize(PCharacter);
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
	}
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
	FVector spawnLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();

	FTransform spawnTransform(spawnRotation, spawnLocation);

	BG3Camera = GetWorld()->SpawnActor<ABG3GameCamera>(BG3CameraClass, spawnTransform);
	BG3Camera->SpringArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
}

void ABG3GameModePlayerController::InitializeCamera()
{
	SpawnCamera();
	SetViewTargetWithBlend(BG3Camera);

	// Set Follow Mode
	FVector location = PossessedCharacter->GetActorLocation();
	BG3Camera->SetActorLocation(location);
	BG3Camera->ChangeViewMode(EGameCameraViewMode::FocusMode);
}

