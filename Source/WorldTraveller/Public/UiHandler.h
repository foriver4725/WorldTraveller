#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UiHandler.generated.h"

UINTERFACE(MinimalAPI)
class UUiHandler : public UInterface
{
	GENERATED_BODY()
};

class WORLDTRAVELLER_API IUiHandler
{
	GENERATED_BODY()

public:
	virtual bool GetUiEnabled() const = 0;
	virtual void SetUiEnabled(bool bEnabled) = 0;
};
