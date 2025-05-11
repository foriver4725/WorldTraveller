#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Home/UI/IUiManager.h"
#include "StartGameUiManager.generated.h"

class UUserWidget;
class UButton;

UCLASS()
class WORLDTRAVELLER_API AStartGameUiManager final : public AActor, public NHome::IUiManager
{
	GENERATED_BODY()

public:
	AStartGameUiManager();

	virtual bool GetUiEnabled() const final;
	virtual void SetUiEnabled(bool bEnabled) final;

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> widgetClass;

	TObjectPtr<UUserWidget> userWidget = nullptr;
	TObjectPtr<UButton> submitButton = nullptr;

	void OnSubmitButtonClicked();
};
