#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CentralUnit.generated.h"

UCLASS()
class WORLDTRAVELLER_API ACentralUnit : public AActor
{
	GENERATED_BODY()
	
public:
	ACentralUnit();

protected:
	virtual void BeginPlay() override;

public:

};
