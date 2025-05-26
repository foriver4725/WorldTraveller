#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Home_FirstLoginHandler.generated.h"

class AHome_UiManager;

UCLASS()
class WORLDTRAVELLER_API AHome_FirstLoginHandler final : public AActor
{
	GENERATED_BODY()

public:
	AHome_FirstLoginHandler();

protected:
	virtual void BeginPlay() override final;
	virtual void Tick(float DeltaTime) override final;

private:
	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<AHome_UiManager> UiManager;

	bool bFirstLogin = false;
	void OnFirstLogin();
	void OnSecondOrLaterLogin();

	bool bFirstTick = true;
};
