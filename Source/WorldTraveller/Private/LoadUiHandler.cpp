#include "LoadUiHandler.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Extensions.h"
#include "PlayerCharacter.h"

ALoadUiHandler::ALoadUiHandler()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALoadUiHandler::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(widgetClass))
	{
		userWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
		if (IsValid(userWidget))
		{
			panel = Cast<UImage>(userWidget->GetWidgetFromName("Panel"));

			userWidget->AddToViewport(MAX_int32);
		}
	}

	playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (IsValid(playerCharacter))
		playerCharacter->SetInputEnabled(false);

	SetUiTransparency(1);
	SetUiEnabled(true);
}

void ALoadUiHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (fadeState == EFadeState::OnGC)
	{
		if (++waitForGCFrameCount >= waitForGCFrames)
		{
			fadeState = EFadeState::FadeIn;
		}
	}
	else if (fadeState == EFadeState::FadeIn)
	{
		using namespace Extensions;

		fadeTime += DeltaTime;
		SetUiTransparency(RemapClamped(fadeTime, 0, fadeInDuration, 1, 0));

		if (fadeTime >= fadeInDuration)
		{
			fadeTime = 0.0f;
			fadeState = EFadeState::InGame;
			SetUiEnabled(false);

			if (IsValid(playerCharacter))
				playerCharacter->SetInputEnabled(true);
		}
	}
	else if (fadeState == EFadeState::FadeOut)
	{
		using namespace Extensions;

		fadeTime += DeltaTime;
		SetUiTransparency(RemapClamped(fadeTime, 0, fadeOutDuration, 0, 1));

		if (fadeTime >= fadeOutDuration)
		{
			fadeTime = 0.0f;
			fadeState = EFadeState::None;

			if (!openingLevelName.IsNone())
				UGameplayStatics::OpenLevel(this, openingLevelName);
		}
	}
}

void ALoadUiHandler::StartFadeIn()
{
	if (fadeState != EFadeState::Waiting) return;
	fadeState = EFadeState::OnGC;

	GEngine->ForceGarbageCollection(true);
}

void ALoadUiHandler::StartFadeOut(const FName& levelNameToOpenOnFadeOutFinished)
{
	if (fadeState != EFadeState::InGame) return;
	fadeState = EFadeState::FadeOut;
	SetUiTransparency(0);
	SetUiEnabled(true);

	if (IsValid(playerCharacter))
		playerCharacter->SetInputEnabled(false);

	openingLevelName = levelNameToOpenOnFadeOutFinished;
}

bool ALoadUiHandler::GetUiEnabled() const
{
	return enabled;
}

void ALoadUiHandler::SetUiEnabled(bool bEnabled)
{
	enabled = bEnabled;

	if (IsValid(userWidget))
	{
		userWidget->SetIsEnabled(enabled);
		userWidget->SetVisibility(enabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void ALoadUiHandler::SetUiTransparency(float alpha)
{
	if (IsValid(panel))
		panel->SetOpacity(alpha);
}
