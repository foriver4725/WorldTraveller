#include "InputManager.h"

AInputManager::AInputManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;
}

void AInputManager::BeginPlay()
{
	Super::BeginPlay();

	if (instance.IsValid())
	{
		this->Destroy();
		return;
	}
	instance = this;
}

void AInputManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bInteractPressed &= false;
	bCancelPressed &= false;
	bEscapePressed &= false;
	bEscapeReleased &= false;
}
