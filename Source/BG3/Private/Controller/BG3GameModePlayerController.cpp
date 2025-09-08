
#include "Controller/BG3GameModePlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "BG3/Public/Actor/BG3GameCamera.h"
#include "Blueprint/UserWidget.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/Widget/CombatActionPanel.h"
#include "UI/WidgetController/CombatActionWidgetController.h"

ABG3GameModePlayerController::ABG3GameModePlayerController()
{
	
}

void ABG3GameModePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// camera setting
	SpawnCamera();
	SetViewTargetWithBlend(BG3Camera);

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

void ABG3GameModePlayerController::SpawnCamera()
{
	FRotator spawnRotation = FRotator(0, 0, 0);
	FVector spawnLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();

	FTransform spawnTransform(spawnRotation, spawnLocation);
	
	BG3Camera = GetWorld()->SpawnActor<ABG3GameCamera>(BG3CameraClass, spawnTransform);
	BG3Camera->SpringArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
}

void ABG3GameModePlayerController::SetCameraLocation(FVector location)
{
	BG3Camera->SetActorLocation(location);
}
