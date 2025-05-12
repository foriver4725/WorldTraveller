#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Home_UiManager.generated.h"

class UUserWidget;
class UImage;
class UTextBlock;
class UButton;

enum class EHome_UiDescriptionTextType : uint8;
enum class EHome_UiType : uint8;

class AHome_StartGameUiHandler;

UCLASS()
class WORLDTRAVELLER_API AHome_UiManager final : public AActor
{
	GENERATED_BODY()

	using DescTextType = EHome_UiDescriptionTextType;
	using UiType = EHome_UiType;

public:
	AHome_UiManager();

	void SetPointerActivation(bool bActivate);
	void SetDescriptionText(DescTextType textType);

	void SetUiEnabled(UiType type, bool bEnabled);

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY(EditInstanceOnly, Category = "Property|UI")
	TObjectPtr<AHome_StartGameUiHandler> startGameUiHandler;

	TObjectPtr<UUserWidget> userWidget = nullptr;
	TObjectPtr<UImage> pointer = nullptr;
	TObjectPtr<UTextBlock> descriptionText = nullptr;

	bool bIsPointerActive = false;
	EHome_UiDescriptionTextType currentDescriptionText;
};
