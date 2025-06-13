#include "Home/UI/Home_Record3dUiHandler.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "SaveGames/SaveGameManager.h"

AHome_Record3dUiHandler::AHome_Record3dUiHandler()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHome_Record3dUiHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bFirstTick) return;
	bFirstTick = false;

	// CoinCollection のUI.
	if (AActor* ui = GetValid(coinCollectionUi))
		if (UWidgetComponent* widgetComponent = Cast<UWidgetComponent>(ui->GetComponentByClass(UWidgetComponent::StaticClass())))
			if (UUserWidget* userWidget = widgetComponent->GetUserWidgetObject())
			{
				// UserWidget を取得できた.

				// セーブデータを取得 (ダメだったら 0 になる).
				uint64 playTimes = 0, maxCoins = 0, totalEarnedStars = 0;
				if (ASaveGameManager* saveGameManager = ASaveGameManager::Instance())
					if (URecordSaveGame* recordSaveGame = Cast<URecordSaveGame>(saveGameManager->Get(ESaveGameType::Record)))
					{
						playTimes = recordSaveGame->Get_CoinCollection_PlayTimes();
						maxCoins = recordSaveGame->Get_CoinCollection_MaxCoins();
						totalEarnedStars = recordSaveGame->Get_CoinCollection_TotalEarnedStars();
					}

				// テキストを更新.
				if (UTextBlock* text = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("PlayTimesText"))))
					text->SetText(FText::AsNumber(playTimes));
				if (UTextBlock* text = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("MaxCoinsText"))))
					text->SetText(FText::AsNumber(maxCoins));
				if (UTextBlock* text = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("TotalEarnedStarsText"))))
					text->SetText(FText::AsNumber(totalEarnedStars));
			}
}
