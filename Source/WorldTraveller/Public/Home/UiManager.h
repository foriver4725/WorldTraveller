#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "UiManager.generated.h"

UCLASS()
class WORLDTRAVELLER_API AUiManager : public AActor
{
	GENERATED_BODY()

public:
	AUiManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;



private:

	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> generalWidgetClass;

	void InitWidgets();

};
