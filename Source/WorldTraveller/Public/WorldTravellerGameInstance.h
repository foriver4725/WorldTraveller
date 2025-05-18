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
	int32 seed = 0;
};
