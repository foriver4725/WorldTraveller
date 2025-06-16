#include "Home/Home_StarAmountShower.h"
#include "SaveGames/SaveGameManager.h"
#include "Home/UI/Home_UiManager.h"

AHome_StarAmountShower::AHome_StarAmountShower()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHome_StarAmountShower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 他のアクタの初期化を待って処理をするためのTickなので、もうTickを呼ぶ必要はない.
	if (!bFirstTick) return;
	bFirstTick = false;

	if (ASaveGameManager* saveGameManager = ASaveGameManager::Instance())
		if (UProgSaveGame* progSaveGame = GetValid(Cast<UProgSaveGame>(saveGameManager->Get(ESaveGameType::Prog))))
			if (!progSaveGame->GetHasPlayedInGame())
			{
				// 未クリア
				if (AHome_UiManager* p = GetValid(UiManager))
				{
					p->SetStarAmountUiEnabled(false);
					p->SetStarAmountText(0);
				}
			}
			else
			{
				// クリア済み
				if (AHome_UiManager* p = GetValid(UiManager))
				{
					p->SetStarAmountUiEnabled(true);
					if (UItemSaveGame* itemSaveGame = GetValid(Cast<UItemSaveGame>(saveGameManager->Get(ESaveGameType::Item))))
						p->SetStarAmountText(itemSaveGame->GetStarAmount());
				}
			}
}
