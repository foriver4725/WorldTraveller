#include "Main/Main_GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Main/Main_Coin.h"
#include "Main/UI/Main_InGameUiHandler.h"
#include "LoadUiHandler.h"
#include "SaveGames/SaveGameManager.h"
#include "SaveGames/ItemSaveGame.h"
#include "Enums/SaveGameType.h"
#include "LevelNames.h"
#include "Extensions.h"

AMain_GameManager::AMain_GameManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMain_GameManager::BeginPlay()
{
	Super::BeginPlay();

	playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void AMain_GameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFirstTick)
	{
		bFirstTick = false;

		if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
		{
			p->SetTimerText(gameLimitTime);
			p->SetTimerTextColor(timerTextColorNormal);
			p->SetTimerTextFontSize(timerTextFontSizeNormal);
			p->SetCoinAmountText(0);
			p->SetDescTextEnabled(false);
			p->SetCountDownText(FText::GetEmpty());
			p->SetEndTextEnabled(false);
			p->SetResultCanvasEnabled(false);
			p->SetResultCoinText(-1);
			p->SetResultStarText(-1);
			p->SetResultCountDownTextEnabled(false);
			p->SetResultCountDownText(backToHomeWaitTime);
		}
	}

	if (state == EState::BeginWait)
	{
		if ((gameTime += DeltaTime) >= beginWaitTime)
		{
			gameTime = 0;
			state = EState::ShowingDescription;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetDescTextEnabled(true);
		}
	}
	else if (state == EState::ShowingDescription)
	{
		if ((gameTime += DeltaTime) >= descriptionTime)
		{
			gameTime = 0;
			state = EState::CountDown_3;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
			{
				p->SetDescTextEnabled(false);
				p->SetCountDownText(FText::FromString(TEXT("3")));
			}
		}
	}
	else if (state == EState::CountDown_3)
	{
		if ((gameTime += DeltaTime) >= countDownInterval)
		{
			gameTime = 0;
			state = EState::CountDown_2;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetCountDownText(FText::FromString(TEXT("2")));
		}
	}
	else if (state == EState::CountDown_2)
	{
		if ((gameTime += DeltaTime) >= countDownInterval)
		{
			gameTime = 0;
			state = EState::CountDown_1;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetCountDownText(FText::FromString(TEXT("1")));
		}
	}
	else if (state == EState::CountDown_1)
	{
		if ((gameTime += DeltaTime) >= countDownInterval)
		{
			gameTime = 0;
			state = EState::CountDown_0;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetCountDownText(FText::FromString(TEXT("Go!")));
		}
	}
	else if (state == EState::CountDown_0)
	{
		if ((gameTime += DeltaTime) >= countDownInterval)
		{
			gameTime = gameLimitTime;
			state = EState::Playing;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetCountDownText(FText::GetEmpty());
		}
	}
	else if (state == EState::Playing)
	{
		if ((gameTime -= DeltaTime) <= finishSoonTime)
		{
			state = EState::Playing_FinishSoon_DoingTransition;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetTimerTextColor(timerTextColorFinishSoon);
		}

		if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
			p->SetTimerText(gameTime);
	}
	else if (state == EState::Playing_FinishSoon_DoingTransition)
	{
		if ((gameTime -= DeltaTime) <= finishSoonTime - onFinishSoonTimerTextScalingDuration)
		{
			state = EState::Playing_FinishSoon;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetTimerTextFontSize(timerTextFontSizeFinishSoon);
		}

		if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
		{
			using namespace Extensions;

			float fontSize = RemapClamped(
				gameTime,
				finishSoonTime, finishSoonTime - onFinishSoonTimerTextScalingDuration,
				timerTextFontSizeNormal, timerTextFontSizeFinishSoon
			);

			p->SetTimerText(gameTime);
			p->SetTimerTextFontSize(fontSize);
		}
	}
	else if (state == EState::Playing_FinishSoon)
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

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetEndTextEnabled(true);
		}

		if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
			p->SetTimerText(gameTime);
	}
	else if (state == EState::EndWait)
	{
		if ((gameTime += DeltaTime) >= endWaitTime)
		{
			gameTime = 0;
			state = EState::ShowingResult_Begin;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
			{
				p->SetEndTextEnabled(false);
				p->SetResultCanvasEnabled(true);
			}
		}
	}
	else if (state == EState::ShowingResult_Begin)
	{
		if ((gameTime += DeltaTime) >= resultUpdateInterval)
		{
			gameTime = 0;
			state = EState::ShowingResult_Coin;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetResultCoinText(coinAmount);
		}
	}
	else if (state == EState::ShowingResult_Coin)
	{
		if ((gameTime += DeltaTime) >= resultUpdateInterval)
		{
			gameTime = 0;
			state = EState::ShowingResult_Star;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetResultStarText(starAmount);
		}
	}
	else if (state == EState::ShowingResult_Star)
	{
		if ((gameTime += DeltaTime) >= resultUpdateInterval)
		{
			gameTime = backToHomeWaitTime;
			state = EState::CountDown_BackToHome;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetResultCountDownTextEnabled(true);
		}
	}
	else if (state == EState::CountDown_BackToHome)
	{
		if ((gameTime -= DeltaTime) <= 0)
		{
			state = EState::Void;

			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetResultCountDownText(gameTime = 0);

			if (IsValid(loadUiHandler))
				loadUiHandler->StartFadeOut(FLevelNames::Home());
		}
		else
		{
			if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
				p->SetResultCountDownText(gameTime);
		}
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

	++coinAmount;
	if (AMain_InGameUiHandler* p = GetValid(inGameUiHandler))
		p->SetCoinAmountText(coinAmount);
}

constexpr uint32 AMain_GameManager::CalcStarFromCoin(uint32 coin)
{
	constexpr uint32 CoinPerStar = 10;
	return coin / CoinPerStar;
}
