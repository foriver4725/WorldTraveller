#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WorldTravellerGameInstance.generated.h"

UCLASS()
class WORLDTRAVELLER_API UWorldTravellerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	int32 GetSeed() const { return seed; }
	void SetSeed(int32 newSeed) { seed = newSeed; }

private:
	// 0 <= seed <= MAX_int32.
	int32 seed = 0;
};
