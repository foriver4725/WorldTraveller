#include "Home/Home_FirstLoginHandler.h"
#include "SaveGames/SaveGameManager.h"
#include "Home/UI/Home_UiManager.h"

AHome_FirstLoginHandler::AHome_FirstLoginHandler()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHome_FirstLoginHandler::BeginPlay()
{
	Super::BeginPlay();

	if (ASaveGameManager* saveGameManager = ASaveGameManager::Instance())
		if (UProgSaveGame* progSaveGame = GetValid(Cast<UProgSaveGame>(saveGameManager->Get(ESaveGameType::Prog))))
			if (!progSaveGame->GetHasLogined())
			{
				progSaveGame->SetHasLogined(true);
				bFirstLogin |= true;
			}
}

void AHome_FirstLoginHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFirstTick)
	{
		bFirstTick = false;

		{
			if (bFirstLogin) this->OnFirstLogin();
			else this->OnSecondOrLaterLogin();
		}

		// 他のアクタの初期化を待って処理をするためのTickなので、もうTickを呼ぶ必要はない.
		PrimaryActorTick.bCanEverTick = false;
	}
}

void AHome_FirstLoginHandler::OnFirstLogin()
{
	if (AHome_UiManager* p = GetValid(UiManager))
	{
		p->SetStarAmountUiEnabled(false);
		p->SetStarAmountText(0);
	}
}

void AHome_FirstLoginHandler::OnSecondOrLaterLogin()
{
	if (AHome_UiManager* p = GetValid(UiManager))
	{
		p->SetStarAmountUiEnabled(true);

		if (ASaveGameManager* saveGameManager = ASaveGameManager::Instance())
			if (UItemSaveGame* itemSaveGame = GetValid(Cast<UItemSaveGame>(saveGameManager->Get(ESaveGameType::Item))))
				p->SetStarAmountText(itemSaveGame->GetStarAmount());
	}
}
