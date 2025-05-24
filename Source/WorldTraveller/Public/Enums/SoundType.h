#pragma once

#include "CoreMinimal.h"
#include "SoundType.generated.h"

UENUM()
enum class ESoundType : uint8
{
	General_ButtonClicked,
	General_ButtonHovered,
	General_TriggerUi,

	Main_Bgm,
	Main_ObtainedCoin,
	Main_DisplayLabel,
	Main_CountDown,
	Main_FinishSoon,
	Main_GameEnded,
	Main_UpdateResult,
};
