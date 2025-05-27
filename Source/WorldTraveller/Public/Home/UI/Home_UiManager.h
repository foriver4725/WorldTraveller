#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UiManager.h"
#include "Home_UiManager.generated.h"

class UUserWidget;
class UImage;
class UTextBlock;
class UButton;
class UCanvasPanel;

enum class EUiDescriptionTextType : uint8;
enum class EUiType : uint8;

class AHome_StartGameUiHandler;

UCLASS()
class WORLDTRAVELLER_API AHome_UiManager final : public AActor, public IUiManager
{
	GENERATED_BODY()

	using DescTextType = EUiDescriptionTextType;
	using UiType = EUiType;

public:
	AHome_UiManager();

	virtual void SetPointerActivation(bool bActivate) override final;
	virtual void SetDescriptionText(DescTextType textType) override final;
	virtual void SetUiEnabled(UiType type, bool bEnabled) override final;

	void SetStarAmountUiEnabled(bool bEnabled);
	void SetStarAmountText(uint64 starAmount);

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY(EditInstanceOnly, Category = "Property|UI")
	TObjectPtr<AHome_StartGameUiHandler> startGameUiHandler;

	UPROPERTY() TObjectPtr<UUserWidget> userWidget = nullptr;
	UPROPERTY() TObjectPtr<UImage> pointer = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> descriptionText = nullptr;
	UPROPERTY() TObjectPtr<UCanvasPanel> starAmountPanel = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> starAmountText = nullptr;

	bool bIsPointerActive = false;
	EUiDescriptionTextType currentDescriptionText;
};
