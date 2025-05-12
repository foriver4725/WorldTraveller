#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Home_UiHandler.generated.h"

UINTERFACE(MinimalAPI)
class UHome_UiHandler : public UInterface
{
	GENERATED_BODY()
};

class WORLDTRAVELLER_API IHome_UiHandler
{
	GENERATED_BODY()

public:
	virtual bool GetUiEnabled() const = 0;
	virtual void SetUiEnabled(bool bEnabled) = 0;
};
