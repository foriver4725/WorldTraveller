#include "Main/Main_GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "PlayerCharacter.h"
#include "Main/Main_Coin.h"
#include "Main/UI/Main_UiZOrders.h"
#include "SaveGames/SaveGameManager.h"
#include "SaveGames/ItemSaveGame.h"
#include "Enums/SaveGameType.h"
#include "LoadUiHandler.h"
#include "LevelNames.h"

AMain_GameManager::AMain_GameManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMain_GameManager::BeginPlay()
{
	Super::BeginPlay();

	playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (UUserWidget* userWidget = CreateWidget(GetWorld(), inGameWidgetClass))
	{
		timerText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("TimerText")));
		coinAmountText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("CoinAmountText")));
		descText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("DescText")));
		countDownText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("CountDownText")));
		endText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("EndText")));
		resultCanvas = userWidget->GetWidgetFromName(TEXT("Result"));
		resultCoinText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("ResultCoinText")));
		resultStarText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("ResultStarText")));
		resultCountDownText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("ResultCountDownText")));

		SetTimerText(gameLimitTime);
		SetCoinAmountText(0);
		SetDescTextEnabled(false);
		SetCountDownText(FText::GetEmpty());
		SetEndTextEnabled(false);
		SetResultCanvasEnabled(false);
		SetResultCoinText(-1);
		SetResultStarText(-1);
		SetResultCountDownTextEnabled(false);
		SetResultCountDownText(backToHomeWaitTime);

		userWidget->AddToViewport(FMain_UiZOrders::InGame);
	}
}

void AMain_GameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (state == EState::BeginWait)
	{
		if ((gameTime += DeltaTime) >= beginWaitTime)
		{
			gameTime = 0;
			state = EState::ShowingDescription;

			SetDescTextEnabled(true);
		}
	}
	else if (state == EState::ShowingDescription)
	{
		if ((gameTime += DeltaTime) >= descriptionTime)
		{
			gameTime = 0;
			state = EState::CountDown_3;

			SetDescTextEnabled(false);
			SetCountDownText(FText::FromString(TEXT("3")));
		}
	}
	else if (state == EState::CountDown_3)
	{
		if ((gameTime += DeltaTime) >= countDownInterval)
		{
			gameTime = 0;
			state = EState::CountDown_2;

			SetCountDownText(FText::FromString(TEXT("2")));
		}
	}
	else if (state == EState::CountDown_2)
	{
		if ((gameTime += DeltaTime) >= countDownInterval)
		{
			gameTime = 0;
			state = EState::CountDown_1;

			SetCountDownText(FText::FromString(TEXT("1")));
		}
	}
	else if (state == EState::CountDown_1)
	{
		if ((gameTime += DeltaTime) >= countDownInterval)
		{
			gameTime = 0;
			state = EState::CountDown_0;

			SetCountDownText(FText::FromString(TEXT("Go!")));
		}
	}
	else if (state == EState::CountDown_0)
	{
		if ((gameTime += DeltaTime) >= countDownInterval)
		{
			gameTime = gameLimitTime;
			state = EState::Playing;

			SetCountDownText(FText::GetEmpty());
		}
	}
	else if (state == EState::Playing)
	{
		if ((gameTime -= DeltaTime) <= 0)
		{
			gameTime = 0;
			state = EState::EndWait;

			// スターの数を計算して、セーブデータに保存する.
			starAmount = CalcStarFromCoin(coinAmount);
			if (ASaveGameManager* saveGameManager = ASaveGameManager::Instance())
				if (UItemSaveGame* itemSaveGame = Cast<UItemSaveGame>(saveGameManager->Get(ESaveGameType::Item)))
					itemSaveGame->AddStarAmount(starAmount);

			SetTimerText(0);
			SetEndTextEnabled(true);
		}
		else
			SetTimerText(gameTime);
	}
	else if (state == EState::EndWait)
	{
		if ((gameTime += DeltaTime) >= endWaitTime)
		{
			gameTime = 0;
			state = EState::ShowingResult_Begin;

			SetEndTextEnabled(false);
			SetResultCanvasEnabled(true);
		}
	}
	else if (state == EState::ShowingResult_Begin)
	{
		if ((gameTime += DeltaTime) >= resultUpdateInterval)
		{
			gameTime = 0;
			state = EState::ShowingResult_Coin;

			SetResultCoinText(coinAmount);
		}
	}
	else if (state == EState::ShowingResult_Coin)
	{
		if ((gameTime += DeltaTime) >= resultUpdateInterval)
		{
			gameTime = 0;
			state = EState::ShowingResult_Star;

			SetResultStarText(starAmount);
		}
	}
	else if (state == EState::ShowingResult_Star)
	{
		if ((gameTime += DeltaTime) >= resultUpdateInterval)
		{
			gameTime = backToHomeWaitTime;
			state = EState::CountDown_BackToHome;

			SetResultCountDownTextEnabled(true);
		}
	}
	else if (state == EState::CountDown_BackToHome)
	{
		if ((gameTime -= DeltaTime) <= 0)
		{
			state = EState::Void;

			SetResultCountDownText(gameTime = 0);

			if (IsValid(loadUiHandler))
				loadUiHandler->StartFadeOut(FLevelNames::Home());
		}
		else
			SetResultCountDownText(gameTime);
	}

	if (state == EState::Playing && (coinGenerateTime += DeltaTime) >= coinGenerateInterval)
	{
		coinGenerateTime -= coinGenerateInterval;
		GenerateNewCoin();
	}
}

void AMain_GameManager::GenerateNewCoin()
{
	if (!IsValid(playerCharacter)) return;

	float th = FMath::DegreesToRadians(FMath::RandRange(0.0f, 360.0f));
	float r = FMath::RandRange(notSpawnRadius, spawnRadius);
	FVector spawnLocation = playerCharacter->GetActorLocation() + FVector(r * FMath::Cos(th), r * FMath::Sin(th), spawnHeight);
	FRotator spawnRotation = FRotator(FQuat(FVector::UpVector, FMath::DegreesToRadians(FMath::RandRange(0.0f, 360.0f))));

	if (AMain_Coin* coinInstance = GetWorld()->SpawnActor<AMain_Coin>(coinOriginal, spawnLocation, spawnRotation))
		coinInstance->OnHitPlayer.BindUObject(this, &AMain_GameManager::OnPlayerGetCoin);
}

void AMain_GameManager::OnPlayerGetCoin()
{
	if (state != EState::Playing) return;
	SetCoinAmountText(++coinAmount);
}

void AMain_GameManager::SetTimerText(float remainTime)
{
	if (!IsValid(timerText)) return;

	int remainTimeInt = FMath::FloorToInt(remainTime);
	int minutes = remainTimeInt / 60;
	int seconds = remainTimeInt % 60;

	timerText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), minutes, seconds)));
}

void AMain_GameManager::SetCoinAmountText(int amount)
{
	if (!IsValid(coinAmountText)) return;
	coinAmountText->SetText(FText::FromString(FString::Printf(TEXT("x %d"), amount)));
}

void AMain_GameManager::SetDescTextEnabled(bool bEnabled)
{
	if (!IsValid(descText)) return;
	descText->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMain_GameManager::SetCountDownText(const FText& text)
{
	if (!IsValid(countDownText)) return;
	countDownText->SetText(text);
}

void AMain_GameManager::SetEndTextEnabled(bool bEnabled)
{
	if (!IsValid(endText)) return;
	endText->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMain_GameManager::SetResultCanvasEnabled(bool bEnabled)
{
	if (!IsValid(resultCanvas)) return;
	resultCanvas->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMain_GameManager::SetResultCoinText(int amount)
{
	if (!IsValid(resultCoinText)) return;
	if (amount == -1)
		resultCoinText->SetText(FText::FromString(TEXT("Coin : ")));
	else
		resultCoinText->SetText(FText::FromString(FString::Printf(TEXT("Coin : %d"), amount)));
}

void AMain_GameManager::SetResultStarText(int amount)
{
	if (!IsValid(resultStarText)) return;
	if (amount == -1)
		resultStarText->SetText(FText::FromString(TEXT("Star : ")));
	else
		resultStarText->SetText(FText::FromString(FString::Printf(TEXT("Star : %d"), amount)));
}

void AMain_GameManager::SetResultCountDownTextEnabled(bool bEnabled)
{
	if (!IsValid(resultCountDownText)) return;
	resultCountDownText->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMain_GameManager::SetResultCountDownText(float remainTime)
{
	if (!IsValid(resultCountDownText)) return;
	resultCountDownText->SetText(FText::FromString(FString::Printf(TEXT("Back to Home in %d sec..."), FMath::FloorToInt(remainTime))));
}

constexpr uint32 AMain_GameManager::CalcStarFromCoin(uint32 coin)
{
	constexpr uint32 CoinPerStar = 10;
	return coin / CoinPerStar;
}
