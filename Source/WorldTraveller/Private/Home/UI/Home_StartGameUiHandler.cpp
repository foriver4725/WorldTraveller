#include "Home/UI/Home_StartGameUiHandler.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "CursorManager.h"
#include "Home/UI/Home_UiZOrders.h"
#include "LevelNames.h"
#include "WorldTravellerGameInstance.h"

AHome_StartGameUiHandler::AHome_StartGameUiHandler()
{
}

void AHome_StartGameUiHandler::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(widgetClass))
	{
		userWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
		if (IsValid(userWidget))
		{
			SetUiEnabled(false);

			submitButton = Cast<UButton>(userWidget->GetWidgetFromName(TEXT("SubmitButton")));
			if (submitButton)
				submitButton->OnClicked.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnSubmitButtonClicked);

			closeButton = Cast<UButton>(userWidget->GetWidgetFromName(TEXT("CloseButton")));
			if (closeButton)
				closeButton->OnClicked.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnCloseButtonClicked);

			userWidget->AddToViewport(FHome_UiZOrders::StartGame);
		}
	}
}

void AHome_StartGameUiHandler::OnSubmitButtonClicked()
{
	if (TObjectPtr<UWorldTravellerGameInstance> gameInstance = Cast<UWorldTravellerGameInstance>(GetGameInstance()))
	{
		if (IsValid(gameInstance))
			gameInstance->SetSeed(12345);
	}

	UGameplayStatics::OpenLevel(GetWorld(), FLevelNames::Main);
}

void AHome_StartGameUiHandler::OnCloseButtonClicked()
{
	SetUiEnabled(false);
}

bool AHome_StartGameUiHandler::GetUiEnabled() const
{
	return enabled;
}

void AHome_StartGameUiHandler::SetUiEnabled(bool bEnabled)
{
	enabled = bEnabled;

	UGameplayStatics::SetGamePaused(GetWorld(), enabled);

	if (IsValid(userWidget))
	{
		userWidget->SetIsEnabled(enabled);
		userWidget->SetVisibility(enabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (IsValid(cursorManager))
		cursorManager->SetCursorEnabled(enabled);
}
