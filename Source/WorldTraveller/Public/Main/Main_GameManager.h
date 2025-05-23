#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Main_GameManager.generated.h"

class UTextBlock;
class UWidget;
class AMain_Coin;
class APlayerCharacter;
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

	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> inGameWidgetClass;

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

	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<ALoadUiHandler> loadUiHandler;

	enum class EState : uint8
	{
		BeginWait,
		ShowingDescription,
		CountDown_3, CountDown_2, CountDown_1, CountDown_0,
		Playing,
		EndWait,
		ShowingResult_Begin, ShowingResult_Coin, ShowingResult_Star,
		CountDown_BackToHome,
		Void,  // ステートの終着点.
	};

	UPROPERTY() TObjectPtr<UTextBlock> timerText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> coinAmountText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> descText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> countDownText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> endText = nullptr;
	UPROPERTY() TObjectPtr<UWidget> resultCanvas = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> resultCoinText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> resultStarText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> resultCountDownText = nullptr;
	UPROPERTY() TObjectPtr<APlayerCharacter> playerCharacter = nullptr;  // プレイヤーの座標をもらうため.

	EState state = EState::BeginWait;
	float coinGenerateTime = 0;
	float gameTime = 0;  // ステート遷移で使う、汎用カウンタ.
	uint32 coinAmount = 0;  // プレイヤーが獲得したコインの数.
	uint32 starAmount = 0;  // プレイヤーが獲得したスターの数.

	void GenerateNewCoin();
	void SetTimerText(float remainTime);
	void SetCoinAmountText(int amount);
	void SetDescTextEnabled(bool bEnabled);
	void SetCountDownText(const FText& text);
	void SetEndTextEnabled(bool bEnabled);
	void SetResultCanvasEnabled(bool bEnabled);
	void SetResultCoinText(int amount);  // if -1, only shows "Coin : ".
	void SetResultStarText(int amount);  // if -1, only shows "Star : ".
	void SetResultCountDownTextEnabled(bool bEnabled);
	void SetResultCountDownText(float remainTime);

	constexpr uint32 CalcStarFromCoin(uint32 coin);

	UFUNCTION()
	void OnPlayerGetCoin();
};
