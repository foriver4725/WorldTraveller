#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Home_StarAmountShower.generated.h"

class AHome_UiManager;

UCLASS()
class WORLDTRAVELLER_API AHome_StarAmountShower final : public AActor
{
	GENERATED_BODY()

public:
	AHome_StarAmountShower();

protected:
	virtual void Tick(float DeltaTime) override final;

private:
	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<AHome_UiManager> UiManager;

	bool bFirstTick = true;
};
