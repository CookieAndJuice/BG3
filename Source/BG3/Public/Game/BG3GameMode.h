
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BG3GameMode.generated.h"

UCLASS()
class BG3_API ABG3GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABG3GameMode();
	
	virtual void BeginPlay() override;
	

};
