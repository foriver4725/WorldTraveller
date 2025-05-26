#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProgSaveGame.generated.h"

UCLASS()
class WORLDTRAVELLER_API UProgSaveGame final : public USaveGame
{
	GENERATED_BODY()

public:
	inline bool GetHasLogined() const { return hasLogined; }
	inline void SetHasLogined(bool b) { hasLogined |= b; }

private:
	// 厳密には、"Home レベルに遷移したことがあるか".
	UPROPERTY() bool hasLogined = false;
};
