#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveGameManager.generated.h"

enum class ESaveGameType : uint8;
class USaveGame;
class UProgSaveGame;
class UItemSaveGame;

UCLASS()
class WORLDTRAVELLER_API ASaveGameManager final : public AActor
{
	GENERATED_BODY()

public:
	ASaveGameManager();

	static inline ASaveGameManager* Instance() { return instance.Get(); }

	void Save(ESaveGameType type);  // Works automatically.
	void Load(ESaveGameType type);  // Works automatically.
	USaveGame* Get(ESaveGameType type);

protected:
	virtual void BeginPlay() override final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

private:

	static constexpr int UserIndex = 0;
	static constexpr float AutoSaveInterval = 30.0f;  // seconds.

	static inline TWeakObjectPtr<ASaveGameManager> instance = nullptr;

	// SaveGames.
	UPROPERTY() TObjectPtr<UProgSaveGame> progSaveGame = nullptr;
	UPROPERTY() TObjectPtr<UItemSaveGame> itemSaveGame = nullptr;

	FTimerHandle autoSaveTimerHandle;

	void AutoSave();
};
