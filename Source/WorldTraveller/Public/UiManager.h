#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UiManager.generated.h"

UINTERFACE(MinimalAPI)
class UUiManager : public UInterface
{
	GENERATED_BODY()
};

enum class EUiDescriptionTextType : uint8;
enum class EUiType : uint8;

class WORLDTRAVELLER_API IUiManager
{
	GENERATED_BODY()

	using DescTextType = EUiDescriptionTextType;
	using UiType = EUiType;

public:
	virtual void SetPointerActivation(bool bActivate) = 0;
	virtual void SetDescriptionText(DescTextType textType) = 0;
	virtual void SetUiEnabled(UiType type, bool bEnabled) = 0;
};
