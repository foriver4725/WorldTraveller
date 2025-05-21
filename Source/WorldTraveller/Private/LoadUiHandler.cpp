#include "LoadUiHandler.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Extensions.h"

ALoadUiHandler::ALoadUiHandler()
{
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

	fadeState = EFadeState::FadeIn;
	SetUiTransparency(1);
	SetUiEnabled(true);
}

void ALoadUiHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (fadeState == EFadeState::FadeIn)
	{
		using namespace Extensions;

		fadeTime += DeltaTime;
		SetUiTransparency(RemapClamped(fadeTime, 0, fadeInDuration, 1, 0));

		if (fadeTime >= fadeInDuration)
		{
			fadeTime = 0.0f;
			fadeState = EFadeState::InGame;
			SetUiEnabled(false);
			// フェードイン完了
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
			// フェードアウト完了(シーン遷移処理)
		}
	}
}

void ALoadUiHandler::StartFadeOut()
{
	if (fadeState != EFadeState::InGame) return;
	fadeState = EFadeState::FadeOut;
	SetUiTransparency(0);
	SetUiEnabled(true);
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
	{
		FLinearColor color = panel->GetColorAndOpacity();
		color.A = alpha;
		panel->SetColorAndOpacity(color);
	}
}
