#include "Home/UI/Home_StartGameUiHandler.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "InputManager.h"
#include "CursorManager.h"
#include "LoadUiHandler.h"
#include "SoundManager.h"
#include "PlayerCharacter.h"
#include "UiZOrders.h"
#include "LevelNames.h"
#include "WorldTravellerGameInstance.h"
#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#endif

static FText ValidateEigenvalueText(const FText& text);
static int32 GetEigenvalueFromText(const FText& text);
static constexpr bool IsOnlyDigits(const TCHAR* str);

AHome_StartGameUiHandler::AHome_StartGameUiHandler()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void AHome_StartGameUiHandler::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(widgetClass))
	{
		if (userWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass))
		{
			containerCanvas = Cast<UCanvasPanel>(userWidget->GetWidgetFromName(TEXT("Container")));

			if (closeButton = Cast<UButton>(userWidget->GetWidgetFromName(TEXT("CloseButton"))))
			{
				closeButton->OnHovered.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnCloseButtonHovered);
				closeButton->OnUnhovered.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnCloseButtonUnhovered);
				closeButton->OnClicked.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnCloseButtonClicked);
			}

			if (closeButtonImage = Cast<UImage>(userWidget->GetWidgetFromName(TEXT("CloseImage"))))
			{
				initCloseButtonImageSize = closeButtonImage->GetBrush().GetImageSize();
			}

			if (UButton* submitButton_CoinCollection =
				Cast<UButton>(userWidget->GetWidgetFromName(TEXT("SubmitButton_CoinCollection"))))
			{
				submitButton_CoinCollection->OnHovered.
					AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnSubmitButtonHovered_CoinCollection);
				submitButton_CoinCollection->OnUnhovered.
					AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnSubmitButtonUnhovered_CoinCollection);
				submitButton_CoinCollection->OnClicked.
					AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnSubmitButtonClicked_CoinCollection);

				submitButtons.Add(EInGameType::CoinCollection, submitButton_CoinCollection);
			}

			if (UTextBlock* submitButtonText_CoinCollection =
				Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("SubmitLabel_CoinCollection"))))
			{
				submitButtonTexts.Add(EInGameType::CoinCollection, submitButtonText_CoinCollection);
				initSubmitButtonTextFontSizes.Add(EInGameType::CoinCollection, submitButtonText_CoinCollection->GetFont().Size);
			}

			if (eigenvalueText = Cast<UEditableTextBox>(userWidget->GetWidgetFromName(TEXT("EigenvalueText"))))
			{
				eigenvalueText->OnTextChanged.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnEigenvalueTextChanged);
			}

			quitText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("QuitText")));

			{
				this->SetUiEnabled(false);
				this->SetQuitTextEnabled(false);
				this->SetQuitText(0);
			}

			userWidget->AddToViewport(FUiZOrders::Home_StartGame);
		}
	}

	quitTime = quitTimeThreshold;
}

void AHome_StartGameUiHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->CheckEscape(DeltaTime);
}

void AHome_StartGameUiHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	submitButtons.Empty();
	submitButtonTexts.Empty();
	initSubmitButtonTextFontSizes.Empty();
}

// 難解すぎるので、後ほどリファクタ必要かも.
// そもそもエスケープのチェックとか、このクラスでするべきか?
void AHome_StartGameUiHandler::CheckEscape(float DeltaTime)
{
	// キャストコストがあるので、最初に取得.
	if (AInputManager* inputManager = AInputManager::Instance())
	{
		if (GetUiEnabled())
		{
			// ちょっとオーバーヘッドがあるけど、妥協する.
			bJustNextFrameAfterUiDisabled = false;
			bDoingQuit = false;
			quitTime = quitTimeThreshold;
			SetQuitTextEnabled(false);

			if (inputManager->GetCancelPressed() || inputManager->GetEscapePressed())
			{
				bJustNextFrameAfterUiDisabled = true;
				SetUiEnabled(false);
				return;
			}
		}
		else
		{
			if (bJustNextFrameAfterUiDisabled)
			{
				bJustNextFrameAfterUiDisabled = false;
				return;  // このフレームは何もしない.
			}

			if (bDoingQuit)
			{
				if (inputManager->GetEscapeReleased())
				{
					bDoingQuit = false;
					SetQuitTextEnabled(false);
				}
				else
				{
					if ((quitTime -= DeltaTime) <= 0)
					{
						this->QuitGame();
						return;
					}
					else
						SetQuitText(quitTime);
				}
			}
			else
			{
				if (inputManager->GetEscapePressed())
				{
					bDoingQuit = true;
					SetQuitTextEnabled(true);
				}
				else
				{
					// ちょっとオーバーヘッドがあるけど、妥協する.
					quitTime = quitTimeThreshold;
					SetQuitTextEnabled(false);
				}
			}
		}
	}
}

void AHome_StartGameUiHandler::OnCloseButtonHovered()
{
	if (ASoundManager* soundManager = ASoundManager::Instance())
		soundManager->Play2D(ESoundType::General_ButtonHovered);

	if (UImage* p = GetValid(closeButtonImage))
	{
		FSlateBrush brush = p->GetBrush();
		brush.ImageSize *= sizeMultiplierOnHovered;
		p->SetBrush(brush);
	}
}

void AHome_StartGameUiHandler::OnCloseButtonUnhovered()
{
	if (UImage* p = GetValid(closeButtonImage))
	{
		FSlateBrush brush = p->GetBrush();
		brush.ImageSize = initCloseButtonImageSize;
		p->SetBrush(brush);
	}
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

		if (UTextBlock* p = GetValid(submitButtonTexts.FindRef(EInGameType::CoinCollection, nullptr)))
		{
			FSlateFontInfo font = p->GetFont();
			font.Size *= sizeMultiplierOnHovered;
			p->SetFont(font);
		}
	}
}

void AHome_StartGameUiHandler::OnSubmitButtonUnhovered(EInGameType type)
{
	if (type == EInGameType::CoinCollection)
	{
		if (UTextBlock* p = GetValid(submitButtonTexts.FindRef(EInGameType::CoinCollection, nullptr)))
		{
			FSlateFontInfo font = p->GetFont();
			font.Size = initSubmitButtonTextFontSizes.FindRef(EInGameType::CoinCollection, 0);
			p->SetFont(font);
		}
	}
}

void AHome_StartGameUiHandler::OnSubmitButtonClicked(EInGameType type)
{
	if (type == EInGameType::CoinCollection)
	{
		if (ASoundManager* soundManager = ASoundManager::Instance())
			soundManager->Play2D(ESoundType::General_ButtonClicked);

		if (UEditableTextBox* textBox = GetValid(eigenvalueText))
		{
			using GI = UWorldTravellerGameInstance;

			int32 eigenvalue = GetEigenvalueFromText(textBox->GetText());
			if (eigenvalue == -1) eigenvalue = FMath::Rand32();

			if (GI* gi = GetValid(Cast<GI>(GetGameInstance())))
				gi->SetSeed(eigenvalue);
		}

		SetUiEnabled(false);

		if (IsValid(loadUiHandler))
			loadUiHandler->StartFadeOut(FLevelNames::Main());
	}
}

void AHome_StartGameUiHandler::OnEigenvalueTextChanged(const FText& text)
{
	FText validatedText = ValidateEigenvalueText(text);

	if (UEditableTextBox* textBox = GetValid(eigenvalueText))
		textBox->SetText(validatedText);
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

	if (UCanvasPanel* p = GetValid(containerCanvas))
	{
		p->SetIsEnabled(enabled);
		p->SetVisibility(enabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (IsValid(cursorManager))
		cursorManager->SetCursorEnabled(enabled);
}

void AHome_StartGameUiHandler::SetQuitTextEnabled(bool bEnabled)
{
	if (UTextBlock* p = GetValid(quitText))
	{
		p->SetIsEnabled(bEnabled);
		p->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void AHome_StartGameUiHandler::SetQuitText(float remainTime)
{
	if (UTextBlock* p = GetValid(quitText))
	{
		FString text = FString::Printf(TEXT("Quit the game in %d sec ..."), FMath::FloorToInt(remainTime));
		p->SetText(FText::FromString(text));
	}
}

void AHome_StartGameUiHandler::QuitGame()
{
#if WITH_EDITOR
	if (GEditor) GEditor->RequestEndPlayMap();
#else
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
#endif
}

// eigenvalueText の入力値を検証し、適切な文字列に変換する.
FText ValidateEigenvalueText(const FText& text)
{
	FString str = text.ToString();

	if (str.Len() <= 0) return FText::GetEmpty();  // 入力が空文字列.
	if (!IsOnlyDigits(*str)) return FText::GetEmpty();  // 入力が数字以外の文字列.

	// 0 <= eigenvalue <= MAX_int32 のため、最大値は 10桁.
	// 11桁以上の数字をトリムする.
	if (str.Len() >= 11)
		str = str.Left(10);

	// eigenvalueの値を、適切な範囲に制限する.
	int64 eigenvalueInt64 = FCString::Atoi64(*str);
	int32 eigenvalue = StaticCast<int32>(FMath::Clamp(eigenvalueInt64, StaticCast<int64>(0), StaticCast<int64>(MAX_int32)));

	return FText::FromString(FString::Printf(TEXT("%d"), eigenvalue));
}

// text から eigenvalue に変換 (無効なら -1 を返し、ランダムの合図とする).
int32 GetEigenvalueFromText(const FText& text)
{
	FString str = text.ToString();

	if (str.Len() <= 0) return -1;  // 文字があるか?
	if (!IsOnlyDigits(*str)) return -1;  // 数字のみか?
	if (str.Len() >= 11) return -1;  // 高々 10 桁か?

	// この時点で一旦数字に変換.
	int64 eigenvalueInt64 = FCString::Atoi64(*str);

	// 0 <= eigenvalue <= MAX_int32 であるか?
	if (eigenvalueInt64 < 0) return -1;
	if (eigenvalueInt64 > MAX_int32) return -1;

	int32 eigenvalue = StaticCast<int32>(eigenvalueInt64);
	return eigenvalue;
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
