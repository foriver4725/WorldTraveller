#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UiHandler.h"
#include "Enums/InGameType.h"
#include "Home_StartGameUiHandler.generated.h"

class UUserWidget;
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

	virtual bool GetUiEnabled() const final;
	virtual void SetUiEnabled(bool bEnabled) final;

protected:
	virtual void BeginPlay() override final;
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

	UPROPERTY() TObjectPtr<UUserWidget> userWidget = nullptr;
	UPROPERTY() TObjectPtr<UButton> closeButton = nullptr;
	UPROPERTY() TObjectPtr<UImage> closeButtonImage = nullptr;
	UPROPERTY() TMap<EInGameType, TObjectPtr<UButton>> submitButtons;
	UPROPERTY() TMap<EInGameType, TObjectPtr<UTextBlock>> submitButtonTexts;
	UPROPERTY() TObjectPtr<UEditableTextBox> eigenvalueText = nullptr;

	FVector2f initCloseButtonImageSize;
	TMap<EInGameType, float> initSubmitButtonTextFontSizes;

	FDelegateHandle onPlayerCancelledHandle;
	bool enabled = true;
	bool bFirstSetEnabled = true;

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
	UFUNCTION() void OnPlayerCancelled();
};
