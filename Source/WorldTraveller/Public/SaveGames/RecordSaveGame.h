#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RecordSaveGame.generated.h"

UCLASS()
class WORLDTRAVELLER_API URecordSaveGame final : public USaveGame
{
	GENERATED_BODY()

public:
	inline uint64 Get_CoinCollection_PlayTimes() const { return coinCollection_playTimes; }
	inline uint64 Get_CoinCollection_MaxCoins() const { return coinCollection_maxCoins; }
	inline uint64 Get_CoinCollection_TotalEarnedStars() const { return coinCollection_totalEarnedStars; }

	inline void Add_CoinCollection_PlayTimes(uint64 amount)
	{
		constexpr uint64 MaxValue = 99999;
		if (coinCollection_playTimes <= MaxValue - amount)
			coinCollection_playTimes += amount;
	}
	inline void Set_CoinCollection_MaxCoins(uint64 amount)
	{
		constexpr uint64 MaxValue = 999;
		amount = FMath::Min(amount, MaxValue);
		coinCollection_maxCoins = FMath::Max(coinCollection_maxCoins, amount);
	}
	inline void Add_CoinCollection_TotalEarnedStars(uint64 amount)
	{
		constexpr uint64 MaxValue = 9999999;
		if (coinCollection_totalEarnedStars <= MaxValue - amount)
			coinCollection_totalEarnedStars += amount;
	}

private:
	UPROPERTY() uint64 coinCollection_playTimes = 0;  // ゲームのトータルプレイ回数 (クリアまでプレイしたもののみ計上).
	UPROPERTY() uint64 coinCollection_maxCoins = 0;  // 1回のプレイで獲得した最大コイン数.
	UPROPERTY() uint64 coinCollection_totalEarnedStars = 0;  // このゲームで獲得した星の総数.
};
