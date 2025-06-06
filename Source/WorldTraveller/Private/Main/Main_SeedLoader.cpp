﻿#include "Main/Main_SeedLoader.h"
#include "Kismet/GamePlayStatics.h"
#include "Math/RandomStream.h"
#include "PlayerCharacter.h"
#include "Engine/DirectionalLight.h"
#include "Components/LightComponent.h"
#include "WorldTravellerGameInstance.h"
#include "Main/UI/Main_UiManager.h"
#include "Main/Main_TerrainGenerator.h"
#include "LoadUiHandler.h"
#include "Extensions.h"

AMain_SeedLoader::AMain_SeedLoader()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMain_SeedLoader::BeginPlay()
{
	Super::BeginPlay();
}

void AMain_SeedLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1フレーム待ってから実行 (他アクタの初期化処理を確実に待つ)
	if (bIsFirstTick)
	{
		bIsFirstTick = false;
		Setup();
	}
}

void AMain_SeedLoader::Setup()
{
	if (TObjectPtr<UWorldTravellerGameInstance> gameInstance = Cast<UWorldTravellerGameInstance>(GetGameInstance()))
	{
		if (!IsValid(gameInstance)) return;

		// シード値を取得して乱数を初期化
		int32 seed = gameInstance->GetSeed();
		FRandomStream rand(seed);

		// 太陽設定
		if (IsValid(sun))
		{
			using namespace Extensions;

			float sunRotY = Remap(rand.RandRange(0, 4), 0, 4, 0.f, -180.f);
			sun->SetActorRotation(FRotator::MakeFromEuler(FVector(0.f, sunRotY, 0.f)));
			sun->GetLightComponent()->SetIntensity(6.f * rand.FRandRange(0.8f, 1.2f));
		}

		// 地形生成
		if (IsValid(terrainGenerator))
			terrainGenerator->GenerateRandomTerrain(rand);

		// ジャンプ力設定
		TObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (IsValid(playerCharacter))
			playerCharacter->RandomizeJumpZVelocityMultiplier(rand);

		// シード値をUIに表示
		if (IsValid(uiManager))
			uiManager->SetSeedText(seed);

		// 初期化処理が終了したので、フェードインを行う
		if (IsValid(loadUiHandler))
			loadUiHandler->StartFadeIn();
	}
}
