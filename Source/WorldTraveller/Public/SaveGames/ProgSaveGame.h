#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProgSaveGame.generated.h"

UCLASS()
class WORLDTRAVELLER_API UProgSaveGame final : public USaveGame
{
	GENERATED_BODY()

public:
	inline bool GetHasPlayedInGame() const { return hasPlayedInGame; }
	inline void AchieveHasPlayedInGame() { hasPlayedInGame |= true; }

private:
	UPROPERTY() bool hasPlayedInGame = false;
};
