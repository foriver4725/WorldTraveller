#include "Main/Main_GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Main/Main_Coin.h"
#include "PlayerCharacter.h"

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
	if ((gameTime += DeltaTime) >= coinGenerateInterval)
	{
		gameTime -= coinGenerateInterval;
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
	UE_LOG(LogTemp, Warning, TEXT("Player Got Coin!"));
}
