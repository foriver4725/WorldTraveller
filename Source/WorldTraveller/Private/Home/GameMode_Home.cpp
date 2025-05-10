#include "Home/GameMode_Home.h"

AGameMode_Home::AGameMode_Home() : Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_MyContents/Blueprints/Character/BP_PlayerCharacter"));
	if (PlayerPawnClassFinder.Succeeded())
		DefaultPawnClass = PlayerPawnClassFinder.Class;
}
