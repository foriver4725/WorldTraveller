#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CursorManager.generated.h"

UCLASS()
class WORLDTRAVELLER_API ACursorManager final : public AActor
{
	GENERATED_BODY()

public:
	ACursorManager();

	void SetCursorEnabled(bool bEnabled);

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY() TObjectPtr<APlayerController> playerController = nullptr;
};
