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

	if (bEnabled)
	{
		playerController->SetShowMouseCursor(true);
		playerController->SetInputMode(FInputModeGameAndUI());

		int32 width, height;
		playerController->GetViewportSize(width, height);
		playerController->SetMouseLocation(width >> 1, height >> 1);
	}
	else
	{
		playerController->SetShowMouseCursor(false);
		playerController->SetInputMode(FInputModeGameOnly());
	}
}
