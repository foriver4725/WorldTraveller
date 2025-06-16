#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ItemSaveGame.generated.h"

UCLASS()
class WORLDTRAVELLER_API UItemSaveGame final : public USaveGame
{
	GENERATED_BODY()

public:
	inline uint64 GetStarAmount() const { return starAmount; }
	inline void AddStarAmount(uint64 amount) { if (starAmount <= MAX_uint64 - amount) starAmount += amount; }

private:
	UPROPERTY() uint64 starAmount = 0;
};
