#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Main_GameManager.generated.h"

class UTextBlock;
class UCanvasPanel;
class APlayerCharacter;
class AMain_Coin;
class AMain_InGameUiHandler;
class ALoadUiHandler;

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

	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<AMain_InGameUiHandler> inGameUiHandler;

	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<ALoadUiHandler> loadUiHandler;

	// ゲーム開始前の待機時間(秒).
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "60.0"))
	float beginWaitTime = 2.0f;

	// ゲームの説明を表示する時間(秒).
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "60.0"))
	float descriptionTime = 3.0f;

	// カウントダウンの間隔(秒).
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float countDownInterval = 1.0f;

	// ゲームの時間制限(秒).
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "1.0", ClampMax = "300.0"))
	float gameLimitTime = 30.0f;

	// ゲームの残り時間が少ない警告を表示する、境界時間(秒).
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "30.0"))
	float finishSoonTime = 5.0f;

	// ゲームの残り時間が少ない警告を表示する時、残り時間のテキストを拡大する遷移時間(秒).
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float onFinishSoonTimerTextScalingDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value")
	FLinearColor timerTextColorNormal = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value")
	FLinearColor timerTextColorFinishSoon = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "300.0"))
	float timerTextFontSizeNormal = 90.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "300.0"))
	float timerTextFontSizeFinishSoon = 180.0f;

	// ゲーム終了後の待機時間(秒).
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "60.0"))
	float endWaitTime = 2.0f;

	// リザルトの情報を更新する間隔(秒).
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float resultUpdateInterval = 1.0f;

	// リザルトからホームに戻るまでの待機時間(秒).
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "60.0"))
	float backToHomeWaitTime = 5.0f;

	// コインの生成間隔(秒).
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0001", ClampMax = "10.0"))
	float coinGenerateInterval = 0.3f;

	// プレイヤーの周りにコインを生成する半径.
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.1", ClampMax = "10000.0"))
	float spawnRadius = 1000.0f;

	// プレイヤーの周りにコインを生成「しない」半径.
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.1", ClampMax = "10000.0"))
	float notSpawnRadius = 100.0f;

	// プレイヤーの上にコインを生成する高さ.
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.1", ClampMax = "1000.0"))
	float spawnHeight = 300.0f;

	enum class EState : uint8
	{
		BeginWait,
		ShowingDescription,
		CountDown_3, CountDown_2, CountDown_1, CountDown_0,
		Playing, Playing_FinishSoon_DoingTransition, Playing_FinishSoon,
		EndWait,
		ShowingResult_Begin, ShowingResult_Coin, ShowingResult_Star,
		CountDown_BackToHome,
		Void,  // ステートの終着点.
	};

	UPROPERTY() TObjectPtr<APlayerCharacter> playerCharacter = nullptr;  // プレイヤーの座標をもらうため.

	bool bFirstTick = true;  // 初回のTickかどうか.
	EState state = EState::BeginWait;
	float coinGenerateTime = 0;
	float gameTime = 0;  // ステート遷移で使う、汎用カウンタ.
	uint32 coinAmount = 0;  // プレイヤーが獲得したコインの数.
	uint32 starAmount = 0;  // プレイヤーが獲得したスターの数.

	void GenerateNewCoin();

	constexpr uint32 CalcStarFromCoin(uint32 coin);

	UFUNCTION()
	void OnPlayerGetCoin();
};
