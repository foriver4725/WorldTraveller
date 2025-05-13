#include "Main/Main_GameMode.h"

AMain_GameMode::AMain_GameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_MyContents/Blueprints/Character/BP_PlayerCharacter"));
	if (PlayerPawnClassFinder.Succeeded())
		DefaultPawnClass = PlayerPawnClassFinder.Class;
}
