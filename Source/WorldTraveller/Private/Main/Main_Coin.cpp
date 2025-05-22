#include "Main/Main_Coin.h"
#include "PlayerCharacter.h"

AMain_Coin::AMain_Coin()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMain_Coin::BeginPlay()
{
	Super::BeginPlay();

	staticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (IsValid(staticMeshComponent))
		staticMeshComponent->OnComponentHit.AddUniqueDynamic(this, &AMain_Coin::OnHit);

	rotateSpeed = rotateSpeedDeg * FMath::RandRange(rotateSpeedMultiplierMin, rotateSpeedMultiplierMax);
}

void AMain_Coin::Tick(float DeltaTime)
{
	if ((autoDestroyTime += DeltaTime) >= autoDestroyDuration)
	{
		this->Destroy();
		return;
	}

	staticMeshComponent->AddWorldRotation(FQuat(FVector::UpVector, rotateSpeed * DeltaTime));
}

void AMain_Coin::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	if (!IsValid(OtherActor) || !OtherActor->IsA(APlayerCharacter::StaticClass()))
		return;

	OnHitPlayer.ExecuteIfBound();

	this->Destroy();
}
