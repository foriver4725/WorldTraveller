#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Home_GameInitializer.generated.h"

class ALoadUiHandler;

UCLASS()
class WORLDTRAVELLER_API AHome_GameInitializer final : public AActor
{
	GENERATED_BODY()

public:
	AHome_GameInitializer();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<ALoadUiHandler> loadUiHandler;
};
