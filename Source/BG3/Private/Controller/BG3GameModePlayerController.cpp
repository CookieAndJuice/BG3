
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
	OverlayWidget = CreateWidget<UOverlayWidget>(this, OverlayWidgetClass);
	OverlayWidget->AddToViewport();
	UOverlayWidgetController* WC = NewObject<UOverlayWidgetController>();
	if (WC)
	{
		if (ABaseCharacter* PCharacter = Cast<ABaseCharacter>(GetCharacter()))
		{
			WC->Initialize(PCharacter);	
		}
		OverlayWidget->SetController(WC);
	}
	
	
}

void ABG3GameModePlayerController::UseSkill(int32 SkillID)
{
	PRINTLOG(TEXT("UseSkill"));
}

void ABG3GameModePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABG3GameModePlayerController::SwitchToPawn(ABaseCharacter* NewCharacter)
{
	if (!NewCharacter) return;

	// possess new character
	auto* GMSubsys = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>();
	GMSubsys->GetCurrentPawn()->DisableInput(this);
	
	Possess(NewCharacter);
	NewCharacter->EnableInput(this);

	// switch camera target
	// change UI skill info
}

void ABG3GameModePlayerController::SpawnCamera()
{
	FRotator spawnRotation = FRotator(0, 0, 0);
	FVector spawnLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();

	FTransform spawnTransform(spawnRotation, spawnLocation);

	// 이것보다는 폰에 부착하도록 만들어보기
	BG3Camera = GetWorld()->SpawnActor<ABG3GameCamera>(BG3CameraClass, spawnTransform);
	BG3Camera->SpringArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
}

void ABG3GameModePlayerController::SetCameraLocation(FVector location)
{
	BG3Camera->SetActorLocation(location);
}
