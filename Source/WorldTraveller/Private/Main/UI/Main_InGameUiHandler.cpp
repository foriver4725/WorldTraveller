#include "Main/UI/Main_InGameUiHandler.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Main/UI/Main_UiZOrders.h"

AMain_InGameUiHandler::AMain_InGameUiHandler()
{
}

void AMain_InGameUiHandler::BeginPlay()
{
	Super::BeginPlay();

	if (userWidget = CreateWidget(GetWorld(), inGameWidgetClass))
	{
		timerText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("TimerText")));
		coinAmountText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("CoinAmountText")));
		descText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("DescText")));
		countDownText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("CountDownText")));
		endText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("EndText")));
		resultCanvas = Cast<UCanvasPanel>(userWidget->GetWidgetFromName(TEXT("Result")));
		resultCoinText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("ResultCoinText")));
		resultStarText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("ResultStarText")));
		resultCountDownText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("ResultCountDownText")));

		userWidget->AddToViewport(FMain_UiZOrders::InGame);
	}
}

bool AMain_InGameUiHandler::GetUiEnabled() const
{
	return enabled;
}

void AMain_InGameUiHandler::SetUiEnabled(bool bEnabled)
{
	enabled = bEnabled;

	if (IsValid(userWidget))
	{
		userWidget->SetIsEnabled(enabled);
		userWidget->SetVisibility(enabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void AMain_InGameUiHandler::SetTimerText(float remainTime)
{
	if (!IsValid(timerText)) return;

	int remainTimeInt = FMath::FloorToInt(remainTime);
	int minutes = remainTimeInt / 60;
	int seconds = remainTimeInt % 60;

	timerText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), minutes, seconds)));
}

void AMain_InGameUiHandler::SetTimerTextAppearance(bool bFinishSoon)
{
	if (UTextBlock* p = GetValid(timerText))
	{
		p->SetColorAndOpacity(bFinishSoon ? FLinearColor::Red : FLinearColor::White);

		FSlateFontInfo fontInfo = p->Font;
		fontInfo.Size = bFinishSoon ? 180 : 90;
		p->SetFont(fontInfo);
	}
}

void AMain_InGameUiHandler::SetCoinAmountText(int amount)
{
	if (!IsValid(coinAmountText)) return;
	coinAmountText->SetText(FText::FromString(FString::Printf(TEXT("x %d"), amount)));
}

void AMain_InGameUiHandler::SetDescTextEnabled(bool bEnabled)
{
	if (!IsValid(descText)) return;
	descText->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMain_InGameUiHandler::SetCountDownText(const FText& text)
{
	if (!IsValid(countDownText)) return;
	countDownText->SetText(text);
}

void AMain_InGameUiHandler::SetEndTextEnabled(bool bEnabled)
{
	if (!IsValid(endText)) return;
	endText->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMain_InGameUiHandler::SetResultCanvasEnabled(bool bEnabled)
{
	if (!IsValid(resultCanvas)) return;
	resultCanvas->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMain_InGameUiHandler::SetResultCoinText(int amount)
{
	if (!IsValid(resultCoinText)) return;
	if (amount == -1)
		resultCoinText->SetText(FText::FromString(TEXT("Coin : ")));
	else
		resultCoinText->SetText(FText::FromString(FString::Printf(TEXT("Coin : %d"), amount)));
}

void AMain_InGameUiHandler::SetResultStarText(int amount)
{
	if (!IsValid(resultStarText)) return;
	if (amount == -1)
		resultStarText->SetText(FText::FromString(TEXT("Star : ")));
	else
		resultStarText->SetText(FText::FromString(FString::Printf(TEXT("Star : %d"), amount)));
}

void AMain_InGameUiHandler::SetResultCountDownTextEnabled(bool bEnabled)
{
	if (!IsValid(resultCountDownText)) return;
	resultCountDownText->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMain_InGameUiHandler::SetResultCountDownText(float remainTime)
{
	if (!IsValid(resultCountDownText)) return;
	resultCountDownText->SetText(FText::FromString(FString::Printf(TEXT("Back to Home in %d sec..."), FMath::FloorToInt(remainTime))));
}
