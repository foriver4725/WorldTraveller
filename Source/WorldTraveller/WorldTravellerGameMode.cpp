// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldTravellerGameMode.h"
#include "WorldTravellerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWorldTravellerGameMode::AWorldTravellerGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
