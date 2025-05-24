#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/SoundType.h"
#include "SoundManager.generated.h"

UCLASS()
class WORLDTRAVELLER_API ASoundManager final : public AActor
{
	GENERATED_BODY()

public:
	ASoundManager();

	static inline ASoundManager* Instance() { return instance.Get(); }

	void Play2D(ESoundType type) const;

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|Data")
	TMap<ESoundType, TObjectPtr<USoundBase>> sounds;

	static inline TWeakObjectPtr<ASoundManager> instance = nullptr;
};
