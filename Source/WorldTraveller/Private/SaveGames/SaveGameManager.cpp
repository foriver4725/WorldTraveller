#include "SaveGames/SaveGameManager.h"
#include "Kismet/GameplayStatics.h"

static USaveGame* Create(ESaveGameType type);
static FName GetSlotName(ESaveGameType type);

ASaveGameManager::ASaveGameManager()
{
}

void ASaveGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (instance.IsValid())
	{
		this->Destroy();
		return;
	}
	instance = this;

	this->Load(ESaveGameType::All);

	GetWorld()->GetTimerManager().SetTimer(autoSaveTimerHandle, this, &ASaveGameManager::AutoSave, AutoSaveInterval, true);
}

void ASaveGameManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	this->Save(ESaveGameType::All);
}

void ASaveGameManager::Save(ESaveGameType type)
{
	if (type == ESaveGameType::Prog)
	{
		if (!IsValid(progSaveGame))
			progSaveGame = Cast<UProgSaveGame>(Create(type));
		if (IsValid(progSaveGame))
			UGameplayStatics::SaveGameToSlot(progSaveGame, GetSlotName(type).ToString(), UserIndex);
	}
	else if (type == ESaveGameType::Item)
	{
		if (!IsValid(itemSaveGame))
			itemSaveGame = Cast<UItemSaveGame>(Create(type));
		if (IsValid(itemSaveGame))
			UGameplayStatics::SaveGameToSlot(itemSaveGame, GetSlotName(type).ToString(), UserIndex);
	}
	else if (type == ESaveGameType::All)
	{
		for (uint8 i = 0; i < static_cast<uint8>(ESaveGameType::All); ++i)
			this->Save(static_cast<ESaveGameType>(i));
	}
}

void ASaveGameManager::Load(ESaveGameType type)
{
	if (type == ESaveGameType::Prog)
	{
		if (UGameplayStatics::DoesSaveGameExist(GetSlotName(type).ToString(), UserIndex))
			progSaveGame = Cast<UProgSaveGame>(UGameplayStatics::LoadGameFromSlot(GetSlotName(type).ToString(), UserIndex));
		else
			progSaveGame = Cast<UProgSaveGame>(Create(type));
	}
	else if (type == ESaveGameType::Item)
	{
		if (UGameplayStatics::DoesSaveGameExist(GetSlotName(type).ToString(), UserIndex))
			itemSaveGame = Cast<UItemSaveGame>(UGameplayStatics::LoadGameFromSlot(GetSlotName(type).ToString(), UserIndex));
		else
			itemSaveGame = Cast<UItemSaveGame>(Create(type));
	}
	else if (type == ESaveGameType::All)
	{
		for (uint8 i = 0; i < static_cast<uint8>(ESaveGameType::All); ++i)
			this->Load(static_cast<ESaveGameType>(i));
	}
}

USaveGame* ASaveGameManager::Get(ESaveGameType type)
{
	if (type == ESaveGameType::Prog) return IsValid(progSaveGame) ? progSaveGame : nullptr;
	if (type == ESaveGameType::Item) return IsValid(itemSaveGame) ? itemSaveGame : nullptr;

	return nullptr;
}

void ASaveGameManager::AutoSave()
{
	this->Save(ESaveGameType::All);
}

static USaveGame* Create(ESaveGameType type)
{
	if (type == ESaveGameType::Prog) return UGameplayStatics::CreateSaveGameObject(UProgSaveGame::StaticClass());
	if (type == ESaveGameType::Item) return UGameplayStatics::CreateSaveGameObject(UItemSaveGame::StaticClass());

	return nullptr;
}

static FName GetSlotName(ESaveGameType type)
{
	if (type == ESaveGameType::Prog) return TEXT("Prog");
	if (type == ESaveGameType::Item) return TEXT("Item");

	return NAME_None;
}
