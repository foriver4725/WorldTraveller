#include "Home/UI/Home_StartGameUiHandler.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "CursorManager.h"
#include "LoadUiHandler.h"
#include "SoundManager.h"
#include "PlayerCharacter.h"
#include "UiZOrders.h"
#include "LevelNames.h"
#include "WorldTravellerGameInstance.h"

constexpr bool IsOnlyDigits(const TCHAR* str);

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

			if (closeButton = Cast<UButton>(userWidget->GetWidgetFromName(TEXT("CloseButton"))))
			{
				closeButton->OnHovered.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnCloseButtonHovered);
				closeButton->OnClicked.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnCloseButtonClicked);
			}

			if (UButton* submitButton_CoinCollection = Cast<UButton>(userWidget->GetWidgetFromName(TEXT("SubmitButton_CoinCollection"))))
			{
				submitButton_CoinCollection->OnHovered.
					AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnSubmitButtonHovered_CoinCollection);
				submitButton_CoinCollection->OnClicked.
					AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnSubmitButtonClicked_CoinCollection);

				submitButtons.Add(EInGameType::CoinCollection, submitButton_CoinCollection);
			}

			if (eigenvalueText = Cast<UEditableTextBox>(userWidget->GetWidgetFromName(TEXT("EigenvalueText"))))
			{
				eigenvalueText->OnTextChanged.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnEigenvalueTextChanged);
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

	submitButtons.Empty();
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

void AHome_StartGameUiHandler::OnSubmitButtonHovered(EInGameType type)
{
	if (type == EInGameType::CoinCollection)
	{
		if (ASoundManager* soundManager = ASoundManager::Instance())
			soundManager->Play2D(ESoundType::General_ButtonHovered);
	}
}

void AHome_StartGameUiHandler::OnSubmitButtonClicked(EInGameType type)
{
	if (type == EInGameType::CoinCollection)
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
}

void AHome_StartGameUiHandler::OnEigenvalueTextChanged(const FText& text)
{
	if (UEditableTextBox* textBox = GetValid(eigenvalueText))
	{
		FString text = textBox->GetText().ToString();

		// 入力が空文字列の場合は何もしない.
		if (text.Len() <= 0) return;

		// 入力が数字以外の文字列の場合は、空文字列にする.
		if (!IsOnlyDigits(*text))
		{
			textBox->SetText(FText::GetEmpty());
			return;
		}

		// 0 <= eigenvalue <= MAX_int32 のため、最大値は 10桁.
		if (text.Len() >= 11)
		{
			// 11桁以上の数字をトリムする.
			text = text.Left(10);
			textBox->SetText(FText::FromString(text));
		}

		// eigenvalueの値を、適切な範囲に制限する.
		int64 eigenvalueInt64 = FCString::Atoi64(*text);
		eigenvalueInt64 = FMath::Clamp(eigenvalueInt64, StaticCast<int64>(0), StaticCast<int64>(MAX_int32));
		int32 eigenvalue = StaticCast<int32>(eigenvalueInt64);
		text = FString::Printf(TEXT("%d"), eigenvalue);
		textBox->SetText(FText::FromString(text));
	}
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

constexpr bool IsOnlyDigits(const TCHAR* str)
{
	while (*str != '\0')
	{
		if (!FChar::IsDigit(*str))
			return false;
		++str;
	}
	return true;
}
