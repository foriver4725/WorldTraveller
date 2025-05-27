#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Main_Coin.generated.h"

DECLARE_DELEGATE(FOnCoinHitPlayer)

UCLASS()
class WORLDTRAVELLER_API AMain_Coin final : public AActor
{
	GENERATED_BODY()

public:
	AMain_Coin();

	FOnCoinHitPlayer OnHitPlayer;

protected:
	virtual void BeginPlay() override final;
	virtual void Tick(float DeltaTime) override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float rotateSpeedDeg = 60.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float rotateSpeedMultiplierMin = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float rotateSpeedMultiplierMax = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "1.0", ClampMax = "100.0"))
	float autoDestroyDuration = 20.0f;

	UPROPERTY() TObjectPtr<UStaticMeshComponent> staticMeshComponent;
	float rotateSpeed = 0;
	float autoDestroyTime = 0;

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
