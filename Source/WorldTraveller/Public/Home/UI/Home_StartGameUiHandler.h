#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UiHandler.h"
#include "Enums/InGameType.h"
#include "Home_StartGameUiHandler.generated.h"

class UUserWidget;
class UCanvasPanel;
class UButton;
class UImage;
class UTextBlock;
class UEditableTextBox;

class ACursorManager;
class ALoadUiHandler;

UCLASS()
class WORLDTRAVELLER_API AHome_StartGameUiHandler final : public AActor, public IUiHandler
{
	GENERATED_BODY()

public:
	AHome_StartGameUiHandler();

	// Container を切り替える.
	virtual bool GetUiEnabled() const final;
	virtual void SetUiEnabled(bool bEnabled) final;

protected:
	virtual void BeginPlay() override final;
	virtual void Tick(float DeltaTime) override final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<ACursorManager> cursorManager;

	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<ALoadUiHandler> loadUiHandler;

	// ホバーした時に、サイズを何倍にするか.
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	float sizeMultiplierOnHovered = 1.2f;

	// エスケープキーを何秒押し続けたら、ゲームを終了するか.
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.01", ClampMax = "10.0"))
	float quitTimeThreshold = 3.0f;

	UPROPERTY() TObjectPtr<UUserWidget> userWidget = nullptr;
	UPROPERTY() TObjectPtr<UCanvasPanel> containerCanvas = nullptr;
	UPROPERTY() TObjectPtr<UButton> closeButton = nullptr;
	UPROPERTY() TObjectPtr<UImage> closeButtonImage = nullptr;
	UPROPERTY() TMap<EInGameType, TObjectPtr<UButton>> submitButtons;
	UPROPERTY() TMap<EInGameType, TObjectPtr<UTextBlock>> submitButtonTexts;
	UPROPERTY() TObjectPtr<UEditableTextBox> eigenvalueText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> quitText = nullptr;

	FVector2f initCloseButtonImageSize;
	TMap<EInGameType, float> initSubmitButtonTextFontSizes;

	FDelegateHandle onPlayerCancelledHandle;
	bool enabled = true;
	bool bFirstSetEnabled = true;

	bool bJustNextFrameAfterUiDisabled = false;  // UI を閉じた後、1フレーム待たないとエスケープを受け付けないようにする.
	bool bDoingQuit = false;
	float quitTime = 0;

	UFUNCTION() void OnCloseButtonHovered();
	UFUNCTION() void OnCloseButtonUnhovered();
	UFUNCTION() void OnCloseButtonClicked();
	UFUNCTION() inline void OnSubmitButtonHovered_CoinCollection() { OnSubmitButtonHovered(EInGameType::CoinCollection); }
	UFUNCTION() inline void OnSubmitButtonUnhovered_CoinCollection() { OnSubmitButtonUnhovered(EInGameType::CoinCollection); }
	UFUNCTION() inline void OnSubmitButtonClicked_CoinCollection() { OnSubmitButtonClicked(EInGameType::CoinCollection); }
	void OnSubmitButtonHovered(EInGameType type);
	void OnSubmitButtonUnhovered(EInGameType type);
	void OnSubmitButtonClicked(EInGameType type);
	UFUNCTION() void OnEigenvalueTextChanged(const FText& text);

	void SetQuitTextEnabled(bool bEnabled);
	void SetQuitText(float remainTime);

	void CheckEscape(float DeltaTime);  // 毎フレーム実行.
	void QuitGame();
};
