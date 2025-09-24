// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/MainMenuWidgetController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/MainMenuWidget.h"
#include "UI/Widget/PressEnterKeyWidget.h"
#include "EnhancedInput/Public/InputMappingContext.h"

AMainMenuWidgetController::AMainMenuWidgetController()
{
	ConstructorHelpers::FObjectFinder<UInputMappingContext> mainMenuIMCRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprints/Input/MainMenu/IMC_MainMenu.IMC_MainMenu'"));
	if (mainMenuIMCRef.Object)
	{
		MainMenuIMC = mainMenuIMCRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> enterKeyRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/MainMenu/IA_MainMenuEnter.IA_MainMenuEnter'"));
	if (enterKeyRef.Object)
	{
		EnterKeyAction = enterKeyRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> spaceBarRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/MainMenu/IA_MainMenuSpaceBar.IA_MainMenuSpaceBar'"));
	if (spaceBarRef.Object)
	{
		SpaceBarAction = spaceBarRef.Object;
	}
	
	ConstructorHelpers::FClassFinder<UPressEnterKeyWidget> pressEnterRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_PressEnterKey.WBP_PressEnterKey_C'"));
	if (pressEnterRef.Succeeded())
	{
		PressEnterKeyWidgetClass = pressEnterRef.Class;
	}
	
	ConstructorHelpers::FClassFinder<UMainMenuWidget> mainMenuRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_MainMenu.WBP_MainMenu_C'"));
	if (mainMenuRef.Succeeded())
	{
		MainMenuWidgetClass = mainMenuRef.Class;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> pressKeySoundRef(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sound/BGM/Title_OP__cut_34sec_.Title_OP__cut_34sec_'"));
	if (pressKeySoundRef.Succeeded())
	{
		PressKeySound = pressKeySoundRef.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> mainMenuSoundRef(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sound/BGM/03_NameInput.03_NameInput'"));
	if (mainMenuSoundRef.Succeeded())
	{
		MainMenuSound = mainMenuSoundRef.Object;
	}
}

void AMainMenuWidgetController::BeginPlay()
{
	Super::BeginPlay();

	auto pc = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()); 
	if (subsys)
	{
		subsys->AddMappingContext(MainMenuIMC, 0);
	}

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();

	UGameplayStatics::PlaySound2D(GetWorld(), PressKeySound);

	SetShowMouseCursor(true);
}

void AMainMenuWidgetController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(EnterKeyAction, ETriggerEvent::Triggered, this, &AMainMenuWidgetController::OnEnterKeyClick);
		EIC->BindAction(SpaceBarAction, ETriggerEvent::Triggered, this, &AMainMenuWidgetController::OnSpaceBarClick);
	}
}