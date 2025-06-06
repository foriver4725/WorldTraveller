#include "Home/Home_RoomUnlocker.h"
#include "Engine/StaticMeshActor.h"
#include "SaveGames/SaveGameManager.h"

AHome_RoomUnlocker::AHome_RoomUnlocker()
{
}

void AHome_RoomUnlocker::BeginPlay()
{
	Super::BeginPlay();

	// ルームが開放できるかチェックし、実行する.
	this->UnlockNorthRoom();
	this->UnlockEastRoom();
	this->UnlockSouthRoom();
	this->UnlockWestRoom();
}

void AHome_RoomUnlocker::UnlockNorthRoom()
{
	ASaveGameManager* saveGameManager = ASaveGameManager::Instance();
	if (!saveGameManager) return;

	UProgSaveGame* saveGame = Cast<UProgSaveGame>(saveGameManager->Get(ESaveGameType::Prog));
	if (!saveGame || !saveGame->GetHasLogined()) return;

	if (AStaticMeshActor* wall = GetValid(northWall))
	{
		UStaticMeshComponent* wallMesh = wall->GetStaticMeshComponent();
		if (!IsValid(wallMesh)) return;

		wallMesh->SetVisibility(false);
		wallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AHome_RoomUnlocker::UnlockEastRoom()
{
}

void AHome_RoomUnlocker::UnlockSouthRoom()
{
}

void AHome_RoomUnlocker::UnlockWestRoom()
{
}
