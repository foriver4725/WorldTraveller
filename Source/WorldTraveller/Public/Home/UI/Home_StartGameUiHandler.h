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

	// ホバーした時に、Close Button のサイズを何倍にするか.
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "1.0", ClampMax = "5.0"))
	float closeButtonMultiplierOnHovered = 1.1f;

	// ホバーした時に、Submit Button のサイズを何倍にするか.
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "1.0", ClampMax = "5.0"))
	float submitButtonMultiplierOnHovered = 1.1f;

	// エスケープキーを何秒押し続けたら、ゲームを終了するか.
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.01", ClampMax = "10.0"))
	float quitTimeThreshold = 3.0f;

	UPROPERTY() TObjectPtr<UUserWidget> userWidget = nullptr;
	UPROPERTY() TObjectPtr<UCanvasPanel> containerCanvas = nullptr;
	UPROPERTY() TObjectPtr<UButton> closeButton = nullptr;
	UPROPERTY() TMap<EInGameType, TObjectPtr<UButton>> submitButtons;
	UPROPERTY() TObjectPtr<UEditableTextBox> eigenvalueText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> quitText = nullptr;

	FVector2D initCloseButtonSize;
	TMap<EInGameType, FVector2D> initSubmitButtonSizes;

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
	UFUNCTION() inline void OnSubmitButtonHovered_Null_1() { OnSubmitButtonHovered(EInGameType::Null_1); }
	UFUNCTION() inline void OnSubmitButtonUnhovered_Null_1() { OnSubmitButtonUnhovered(EInGameType::Null_1); }
	UFUNCTION() inline void OnSubmitButtonClicked_Null_1() { OnSubmitButtonClicked(EInGameType::Null_1); }
	UFUNCTION() inline void OnSubmitButtonHovered_Null_2() { OnSubmitButtonHovered(EInGameType::Null_2); }
	UFUNCTION() inline void OnSubmitButtonUnhovered_Null_2() { OnSubmitButtonUnhovered(EInGameType::Null_2); }
	UFUNCTION() inline void OnSubmitButtonClicked_Null_2() { OnSubmitButtonClicked(EInGameType::Null_2); }
	UFUNCTION() inline void OnSubmitButtonHovered_Null_3() { OnSubmitButtonHovered(EInGameType::Null_3); }
	UFUNCTION() inline void OnSubmitButtonUnhovered_Null_3() { OnSubmitButtonUnhovered(EInGameType::Null_3); }
	UFUNCTION() inline void OnSubmitButtonClicked_Null_3() { OnSubmitButtonClicked(EInGameType::Null_3); }
	void OnSubmitButtonHovered(EInGameType type);
	void OnSubmitButtonUnhovered(EInGameType type);
	void OnSubmitButtonClicked(EInGameType type);
	UFUNCTION() void OnEigenvalueTextChanged(const FText& text);

	void SetQuitTextEnabled(bool bEnabled);
	void SetQuitText(float remainTime);

	void CheckEscape(float DeltaTime);  // 毎フレーム実行.
	void QuitGame();
};
