
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "BG3GameModePlayerController.generated.h"

class UOverlayWidget;
class UCombatActionPanel;

UCLASS()
class BG3_API ABG3GameModePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABG3GameModePlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

public:	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> PlayerIMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputAction> CameraMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputAction> CameraZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputAction> CameraRotateAction;

	void OnMoveCamera(const FInputActionValue& value);

	void OnZoomCamera(const FInputActionValue& value);

	void OnRotateCamera(const FInputActionValue& value);
	
public:	// Manager Classes
	UPROPERTY()
	TObjectPtr<class UBG3GameManageSubsystem> GMSubsystem;
	
public:	// Pawn
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class ABaseCharacter> PossessedCharacter;
	
	void SwitchToPawn(class ABaseCharacter* NewCharacter);
	
public:	// Camera
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class ABG3GameCamera> BG3CameraClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class ABG3GameCamera> BG3Camera;
	
	void SpawnCamera();

	void InitializeCamera();

	/* Widget */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UOverlayWidget> OverlayWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UOverlayWidget> OverlayWidget; 
	

protected:
	virtual void BeginPlay() override;

private:
	void UseSkill(int32 SkillID);
	
};
