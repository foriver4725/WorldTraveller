#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UiHandler.h"
#include "LoadUiHandler.generated.h"

class UUserWidget;
class UImage;
class APlayerCharacter;

UCLASS()
class WORLDTRAVELLER_API ALoadUiHandler final : public AActor, public IUiHandler
{
	GENERATED_BODY()

public:
	ALoadUiHandler();

	void StartFadeIn();
	void StartFadeOut(const FName& levelNameToOpenOnFadeOutFinished);

protected:
	virtual void BeginPlay() override final;
	virtual void Tick(float DeltaTime) override final;

	virtual bool GetUiEnabled() const override final;
	virtual void SetUiEnabled(bool bEnabled) override final;

private:

	// 必ず上から下の順に遷移し、逆行することはない.
	enum class EFadeState : uint8
	{
		Waiting,
		FadeIn,
		InGame,
		FadeOut,
		None,  // どこにも遷移できない、「終着点」のステート.
	};

	UPROPERTY(EditDefaultsOnly, Category = "Property|UI")
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float fadeInDuration = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float fadeOutDuration = 1.0f;

	void SetUiTransparency(float alpha);

	UPROPERTY() TObjectPtr<APlayerCharacter> playerCharacter = nullptr;
	UPROPERTY() TObjectPtr<UUserWidget> userWidget = nullptr;
	UPROPERTY() TObjectPtr<UImage> panel = nullptr;

	EFadeState fadeState = EFadeState::Waiting;
	float fadeTime = 0.0f;
	FName openingLevelName = NAME_None;

	bool enabled = false;
};
