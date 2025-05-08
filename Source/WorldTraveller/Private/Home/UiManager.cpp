#include "Home/UiManager.h"
#include "Blueprint/UserWidget.h"

AUiManager::AUiManager()
{
}

void AUiManager::BeginPlay()
{
	Super::BeginPlay();

	InitWidgets();
}

void AUiManager::InitWidgets()
{
	if (IsValid(generalWidgetClass))
	{
		UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), generalWidgetClass);
		if (widget)
			widget->AddToViewport();
	}
}
