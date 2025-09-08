
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BG3GameModePlayerController.generated.h"

class UCombatActionPanel;

UCLASS()
class BG3_API ABG3GameModePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABG3GameModePlayerController();
	virtual void Tick(float DeltaTime) override;

public:	// Pawn
	void SwitchToPawn(class ABaseCharacter* NewCharacter);
	
public:	// camera
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class ABG3GameCamera> BG3CameraClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class ABG3GameCamera> BG3Camera;
	
	void SpawnCamera();

	UFUNCTION(BlueprintCallable)
	void SetCameraLocation(FVector location);


	/* Widget */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCombatActionPanel> ActionPanelClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCombatActionPanel> ActionPanel; 
	

protected:
	virtual void BeginPlay() override;

	
	
};
