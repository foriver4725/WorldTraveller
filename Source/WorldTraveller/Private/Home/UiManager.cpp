#include "Home/UiManager.h"

AUiManager::AUiManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AUiManager::BeginPlay()
{
	Super::BeginPlay();

	InitWidgets();

}

void AUiManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUiManager::InitWidgets()
{
	if (IsValid(generalWidgetClass))
	{
		UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), generalWidgetClass);
		if (IsValid(widget))
			widget->AddToViewport();
	}
}
