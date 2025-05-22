#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProgSaveGame.generated.h"

UCLASS()
class WORLDTRAVELLER_API UProgSaveGame final : public USaveGame
{
	GENERATED_BODY()

public:
	inline bool GetHasFirstLogined() const { return hasFirstLogined; }
	inline void SetHasFirstLogined(bool b) { hasFirstLogined = b; }

private:
	UPROPERTY() bool hasFirstLogined = false;
};
