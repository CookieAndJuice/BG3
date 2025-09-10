
#include "BG3/Public/Game/BG3GameMode.h"

#include "GameFramework/SpectatorPawn.h"

ABG3GameMode::ABG3GameMode()
{
	DefaultPawnClass = ASpectatorPawn::StaticClass();
}

void ABG3GameMode::BeginPlay()
{
	Super::BeginPlay();
}


