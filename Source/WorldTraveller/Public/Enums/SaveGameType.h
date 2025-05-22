#pragma once

#include "CoreMinimal.h"
#include "SaveGameType.generated.h"

UENUM()
enum class ESaveGameType : uint8
{
	Prog,
	Item,
	All,
};
