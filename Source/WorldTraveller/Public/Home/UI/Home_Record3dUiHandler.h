#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Home_Record3dUiHandler.generated.h"

UCLASS()
class WORLDTRAVELLER_API AHome_Record3dUiHandler final : public AActor
{
	GENERATED_BODY()

public:
	AHome_Record3dUiHandler();

protected:
	virtual void Tick(float DeltaTime) override final;

private:
	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<AActor> coinCollectionUi;

	bool bFirstTick = true;
};
