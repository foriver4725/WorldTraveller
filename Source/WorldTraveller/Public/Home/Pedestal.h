#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pedestal.generated.h"

UCLASS()
class WORLDTRAVELLER_API APedestal : public AActor
{
	GENERATED_BODY()

public:
	APedestal();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
