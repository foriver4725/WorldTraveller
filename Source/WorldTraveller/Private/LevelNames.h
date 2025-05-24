#pragma once

#include "CoreMinimal.h"

static struct FLevelNames
{
	static FORCEINLINE FName Home() { return TEXT("Home"); }
	static FORCEINLINE FName Main() { return TEXT("Main"); }
};
