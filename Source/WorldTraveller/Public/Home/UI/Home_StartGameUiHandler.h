#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UiHandler.h"
#include "Enums/InGameType.h"
#include "Home_StartGameUiHandler.generated.h"

class UUserWidget;
class UButton;
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

	UPROPERTY() TObjectPtr<UUserWidget> userWidget = nullptr;
	UPROPERTY() TObjectPtr<UButton> closeButton = nullptr;
	UPROPERTY() TMap<EInGameType, TObjectPtr<UButton>> submitButtons;
	UPROPERTY() TObjectPtr<UEditableTextBox> eigenvalueText = nullptr;

	FDelegateHandle onPlayerCancelledHandle;
	bool enabled = true;
	bool bFirstSetEnabled = true;

	UFUNCTION() void OnCloseButtonHovered();
	UFUNCTION() void OnCloseButtonClicked();
	UFUNCTION() inline void OnSubmitButtonHovered_CoinCollection() { OnSubmitButtonHovered(EInGameType::CoinCollection); }
	UFUNCTION() inline void OnSubmitButtonClicked_CoinCollection() { OnSubmitButtonClicked(EInGameType::CoinCollection); }
	void OnSubmitButtonHovered(EInGameType type);
	void OnSubmitButtonClicked(EInGameType type);
	UFUNCTION() void OnEigenvalueTextChanged(const FText& text);
	UFUNCTION() void OnPlayerCancelled();
};
