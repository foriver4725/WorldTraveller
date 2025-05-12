#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Home/UI/Home_UiHandler.h"
#include "Home_StartGameUiHandler.generated.h"

class UUserWidget;
class UButton;

class ACursorManager;

UCLASS()
class WORLDTRAVELLER_API AHome_StartGameUiHandler final : public AActor, public IHome_UiHandler
{
	GENERATED_BODY()

public:
	AHome_StartGameUiHandler();

	virtual bool GetUiEnabled() const final;
	virtual void SetUiEnabled(bool bEnabled) final;

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<ACursorManager> cursorManager;

	UPROPERTY() TObjectPtr<UUserWidget> userWidget = nullptr;
	UPROPERTY() TObjectPtr<UButton> submitButton = nullptr;
	UPROPERTY() TObjectPtr<UButton> closeButton = nullptr;

	bool enabled = false;

	UFUNCTION() void OnSubmitButtonClicked();
	UFUNCTION() void OnCloseButtonClicked();
};
