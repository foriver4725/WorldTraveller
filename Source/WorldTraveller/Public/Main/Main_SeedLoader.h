#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Main_SeedLoader.generated.h"

class AMain_UiManager;
class AMain_TerrainGenerator;
class ADirectionalLight;
class ALoadUiHandler;

UCLASS()
class WORLDTRAVELLER_API AMain_SeedLoader final : public AActor
{
	GENERATED_BODY()

public:
	AMain_SeedLoader();

protected:
	virtual void BeginPlay() override final;
	virtual void Tick(float DeltaTime) override final;

private:
	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<AMain_UiManager> uiManager;

	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<AMain_TerrainGenerator> terrainGenerator;

	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<ADirectionalLight> sun;

	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<ALoadUiHandler> loadUiHandler;

	bool bIsFirstTick = true;

	void Setup();
};
