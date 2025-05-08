#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pedestal.generated.h"

class UShapeComponent;

UCLASS()
class WORLDTRAVELLER_API APedestal : public AActor
{
	GENERATED_BODY()

public:
	APedestal();

protected:
	virtual void BeginPlay() override final;

private:
	TObjectPtr<UShapeComponent> collision;

	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
