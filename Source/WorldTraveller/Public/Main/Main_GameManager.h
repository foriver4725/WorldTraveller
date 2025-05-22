#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Main_GameManager.generated.h"

class AMain_Coin;
class APlayerCharacter;

UCLASS()
class WORLDTRAVELLER_API AMain_GameManager final : public AActor
{
	GENERATED_BODY()

public:
	AMain_GameManager();

protected:
	virtual void BeginPlay() override final;
	virtual void Tick(float DeltaTime) override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|Dependency")
	TSubclassOf<AMain_Coin> coinOriginal;

	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> inGameWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0001", ClampMax = "10.0"))
	float coinGenerateInterval = 0.3f;

	// プレイヤーの周りにコインを生成する半径.
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.1", ClampMax = "10000.0"))
	float spawnRadius = 1000.0f;

	// プレイヤーの上にコインを生成する高さ.
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.1", ClampMax = "1000.0"))
	float spawnHeight = 300.0f;

	// プレイヤーの座標をもらうため.
	UPROPERTY() TObjectPtr<APlayerCharacter> playerCharacter = nullptr;
	float gameTime = 0;

	void GenerateNewCoin();

	UFUNCTION()
	void OnPlayerGetCoin();
};
