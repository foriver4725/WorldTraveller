#include "Home/Home_GameMode.h"

AHome_GameMode::AHome_GameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_MyContents/Blueprints/Character/BP_PlayerCharacter"));
	if (PlayerPawnClassFinder.Succeeded())
		DefaultPawnClass = PlayerPawnClassFinder.Class;
}
