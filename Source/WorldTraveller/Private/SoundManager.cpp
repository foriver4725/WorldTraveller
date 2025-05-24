#include "SoundManager.h"
#include "Kismet/GameplayStatics.h"

ASoundManager::ASoundManager()
{
}

void ASoundManager::BeginPlay()
{
	Super::BeginPlay();

	if (instance.IsValid())
	{
		this->Destroy();
		return;
	}
	instance = this;
}

void ASoundManager::Play2D(ESoundType type) const
{
	if (USoundBase* sound = sounds[type])
		UGameplayStatics::PlaySound2D(GetWorld(), sound);
}
