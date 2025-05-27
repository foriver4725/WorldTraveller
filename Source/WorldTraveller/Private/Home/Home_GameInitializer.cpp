#include "Home/Home_GameInitializer.h"
#include "LoadUihandler.h"

AHome_GameInitializer::AHome_GameInitializer()
{
}

void AHome_GameInitializer::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(loadUiHandler))
		loadUiHandler->StartFadeIn();
}
