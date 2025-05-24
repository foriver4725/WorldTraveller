#include "Home/UI/Home_StartGameUiHandler.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "CursorManager.h"
#include "LoadUiHandler.h"
#include "SoundManager.h"
#include "PlayerCharacter.h"
#include "UiZOrders.h"
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

			if (submitButton = Cast<UButton>(userWidget->GetWidgetFromName(TEXT("SubmitButton"))))
			{
				submitButton->OnHovered.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnSubmitButtonHovered);
				submitButton->OnClicked.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnSubmitButtonClicked);
			}

			if (closeButton = Cast<UButton>(userWidget->GetWidgetFromName(TEXT("CloseButton"))))
			{
				closeButton->OnHovered.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnCloseButtonHovered);
				closeButton->OnClicked.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnCloseButtonClicked);
			}

			userWidget->AddToViewport(FUiZOrders::Home_StartGame);
		}
	}

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		onPlayerCancelledHandle = playerCharacter->OnPlayerCancelled.AddUObject(this, &AHome_StartGameUiHandler::OnPlayerCancelled);
}

void AHome_StartGameUiHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		playerCharacter->OnPlayerCancelled.Remove(onPlayerCancelledHandle);
}

void AHome_StartGameUiHandler::OnSubmitButtonHovered()
{
	if (ASoundManager* soundManager = ASoundManager::Instance())
		soundManager->Play2D(ESoundType::General_ButtonHovered);
}

void AHome_StartGameUiHandler::OnSubmitButtonClicked()
{
	if (ASoundManager* soundManager = ASoundManager::Instance())
		soundManager->Play2D(ESoundType::General_ButtonClicked);

	if (TObjectPtr<UWorldTravellerGameInstance> gameInstance = Cast<UWorldTravellerGameInstance>(GetGameInstance()))
	{
		if (IsValid(gameInstance))
			gameInstance->SetSeed(FMath::Rand32());
	}

	SetUiEnabled(false);

	if (IsValid(loadUiHandler))
		loadUiHandler->StartFadeOut(FLevelNames::Main());
}

void AHome_StartGameUiHandler::OnCloseButtonHovered()
{
	if (ASoundManager* soundManager = ASoundManager::Instance())
		soundManager->Play2D(ESoundType::General_ButtonHovered);
}

void AHome_StartGameUiHandler::OnCloseButtonClicked()
{
	if (ASoundManager* soundManager = ASoundManager::Instance())
		soundManager->Play2D(ESoundType::General_ButtonClicked);

	SetUiEnabled(false);
}

void AHome_StartGameUiHandler::OnPlayerCancelled()
{
	SetUiEnabled(false);
}

bool AHome_StartGameUiHandler::GetUiEnabled() const
{
	return enabled;
}

void AHome_StartGameUiHandler::SetUiEnabled(bool bEnabled)
{
	if (enabled == bEnabled) return;
	enabled = bEnabled;

	// BeginPlayで初期化するため、この時の実行では音を鳴らさない.
	if (bFirstSetEnabled)
		bFirstSetEnabled = false;
	else
		if (ASoundManager* soundManager = ASoundManager::Instance())
			soundManager->Play2D(ESoundType::General_TriggerUi);

	UGameplayStatics::SetGamePaused(GetWorld(), enabled);

	if (IsValid(userWidget))
	{
		userWidget->SetIsEnabled(enabled);
		userWidget->SetVisibility(enabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (IsValid(cursorManager))
		cursorManager->SetCursorEnabled(enabled);
}
