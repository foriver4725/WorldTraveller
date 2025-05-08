#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UiManager.generated.h"

class UUserWidget;

UCLASS()
class WORLDTRAVELLER_API AUiManager : public AActor
{
	GENERATED_BODY()

public:
	AUiManager();

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> generalWidgetClass;

	void InitWidgets();
};
