#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UiHandler.h"
#include "Main_InGameUiHandler.generated.h"

class UTextBlock;
class UCanvasPanel;

UCLASS()
class WORLDTRAVELLER_API AMain_InGameUiHandler final : public AActor, public IUiHandler
{
	GENERATED_BODY()

public:
	AMain_InGameUiHandler();

	virtual bool GetUiEnabled() const override final;
	virtual void SetUiEnabled(bool bEnabled) override final;

	void SetTimerText(float remainTime);
	void SetTimerTextAppearance(bool bFinishSoon);
	void SetCoinAmountText(int amount);
	void SetDescTextEnabled(bool bEnabled);
	void SetCountDownText(const FText& text);
	void SetEndTextEnabled(bool bEnabled);
	void SetResultCanvasEnabled(bool bEnabled);
	void SetResultCoinText(int amount);  // if -1, only shows "Coin : ".
	void SetResultStarText(int amount);  // if -1, only shows "Star : ".
	void SetResultCountDownTextEnabled(bool bEnabled);
	void SetResultCountDownText(float remainTime);

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> inGameWidgetClass;

	UPROPERTY() TObjectPtr<UUserWidget> userWidget = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> timerText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> coinAmountText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> descText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> countDownText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> endText = nullptr;
	UPROPERTY() TObjectPtr<UCanvasPanel> resultCanvas = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> resultCoinText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> resultStarText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> resultCountDownText = nullptr;

	bool enabled = true;
};
