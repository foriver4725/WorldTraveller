#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Home/UI/Home_UiHandler.h"
#include "Home_StartGameUiHandler.generated.h"

class UUserWidget;
class UButton;

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

	TObjectPtr<UUserWidget> userWidget = nullptr;
	TObjectPtr<UButton> submitButton = nullptr;

	bool enabled = false;

	UFUNCTION()
	void OnSubmitButtonClicked();
};
