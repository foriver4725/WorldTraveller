#include "Home/Home_RoomUnlocker.h"
#include "Engine/StaticMeshActor.h"
#include "SaveGames/SaveGameManager.h"

static void SetActive(AActor* actor, bool bActive);

AHome_RoomUnlocker::AHome_RoomUnlocker()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHome_RoomUnlocker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bFirstTick) return;
	bFirstTick = false;

	// ルームが開放できるかチェックし、実行する.
	this->UnlockNorthRoom();
	this->UnlockEastRoom();
	this->UnlockSouthRoom();
	this->UnlockWestRoom();
}

void AHome_RoomUnlocker::UnlockNorthRoom()
{
	if (ASaveGameManager* saveGameManager = ASaveGameManager::Instance())
		if (UProgSaveGame* saveGame = Cast<UProgSaveGame>(saveGameManager->Get(ESaveGameType::Prog)))
		{
			if (saveGame->GetHasPlayedInGame())
			{
				if (AStaticMeshActor* wall = GetValid(northWall))
					SetActive(wall, false);
			}
		}
}

void AHome_RoomUnlocker::UnlockEastRoom()
{
	if (ASaveGameManager* saveGameManager = ASaveGameManager::Instance())
		if (UProgSaveGame* saveGame = Cast<UProgSaveGame>(saveGameManager->Get(ESaveGameType::Prog)))
		{
			if (!saveGame->GetHasPlayedInGame())
			{
				if (AActor* unlockUi = GetValid(eastUnlockUi))
					SetActive(unlockUi, false);
			}
		}
}

void AHome_RoomUnlocker::UnlockSouthRoom()
{
}

void AHome_RoomUnlocker::UnlockWestRoom()
{
}

// IsValid はチェックしない.
void SetActive(AActor* actor, bool bActive)
{
	if (actor)
	{
		actor->SetActorHiddenInGame(!bActive);
		actor->SetActorEnableCollision(bActive);
	}
}
