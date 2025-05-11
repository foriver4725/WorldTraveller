#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UiManager_Home.generated.h"

class UUserWidget;
class UImage;
class UTextBlock;
class UButton;

namespace NHome
{
	enum class EDescriptionText : uint8;
	enum class EUiType : uint8;
}

class AStartGameUiManager;

UCLASS()
class WORLDTRAVELLER_API AUiManager_Home final : public AActor
{
	GENERATED_BODY()

public:
	AUiManager_Home();

	void SetPointerActivation(bool bActivate);
	void SetDescriptionText(NHome::EDescriptionText textType);

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY(EditInstanceOnly, Category = "Property|UI")
	TObjectPtr<AStartGameUiManager> startGameUiManager;

	TObjectPtr<UUserWidget> userWidget = nullptr;
	TObjectPtr<UImage> pointer = nullptr;
	TObjectPtr<UTextBlock> descriptionText = nullptr;

	bool bIsPointerActive = false;
	NHome::EDescriptionText currentDescriptionText;

	void SetUiEnabled(NHome::EUiType type, bool bEnabled);
};
