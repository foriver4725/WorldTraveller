#include "CursorManager.h"

ACursorManager::ACursorManager()
{
}

void ACursorManager::BeginPlay()
{
	Super::BeginPlay();

	playerController = GetWorld()->GetFirstPlayerController();
	SetCursorEnabled(false);
}

void ACursorManager::SetCursorEnabled(bool bEnabled)
{
	if (!IsValid(playerController))
		return;

	playerController->bShowMouseCursor = bEnabled;

	if (bEnabled)
		playerController->SetInputMode(FInputModeGameAndUI());
	else
		playerController->SetInputMode(FInputModeGameOnly());
}
