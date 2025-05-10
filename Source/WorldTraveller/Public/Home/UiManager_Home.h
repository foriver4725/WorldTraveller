#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EDescriptionText.h"
#include "UiManager_Home.generated.h"

class UUserWidget;
class UImage;
class UTextBlock;

UCLASS()
class WORLDTRAVELLER_API AUiManager_Home final : public AActor
{
	GENERATED_BODY()

public:
	AUiManager_Home();

	void SetPointerActivation(bool bActivate);
	void SetDescriptionText(EDescriptionText textType);

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> widgetClass;

	TObjectPtr<UImage> pointer = nullptr;
	TObjectPtr<UTextBlock> descriptionText = nullptr;

	bool bIsPointerActive = false;
	EDescriptionText currentDescriptionText = EDescriptionText::None;
};
