#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UiManager.h"
#include "Main_UiManager.generated.h"

class UUserWidget;
class UImage;
class UTextBlock;
class UButton;

enum class EUiDescriptionTextType : uint8;
enum class EUiType : uint8;

UCLASS()
class WORLDTRAVELLER_API AMain_UiManager final : public AActor, public IUiManager
{
	GENERATED_BODY()

	using DescTextType = EUiDescriptionTextType;
	using UiType = EUiType;

public:
	AMain_UiManager();

	virtual void SetPointerActivation(bool bActivate) override final;
	virtual void SetDescriptionText(DescTextType textType) override final;
	virtual void SetUiEnabled(UiType type, bool bEnabled) override final;

	void SetSeedText(int32 seed);

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY() TObjectPtr<UUserWidget> userWidget = nullptr;
	UPROPERTY() TObjectPtr<UImage> pointer = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> descriptionText = nullptr;
	UPROPERTY() TObjectPtr<UTextBlock> coordinatesText = nullptr;

	bool bIsPointerActive = false;
	EUiDescriptionTextType currentDescriptionText;
};
