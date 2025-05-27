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
		staticMeshComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AMain_Coin::OnBeginOverlap);

	rotateSpeed = rotateSpeedDeg * FMath::RandRange(rotateSpeedMultiplierMin, rotateSpeedMultiplierMax);
}

void AMain_Coin::Tick(float DeltaTime)
{
	if ((autoDestroyTime += DeltaTime) >= autoDestroyDuration)
	{
		this->Destroy();
		return;
	}

	staticMeshComponent->AddWorldRotation(FQuat(FVector::UpVector, FMath::DegreesToRadians(rotateSpeed) * DeltaTime));
}

void AMain_Coin::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!IsValid(OtherActor) || !OtherActor->IsA(APlayerCharacter::StaticClass()))
		return;

	OnHitPlayer.ExecuteIfBound();

	this->Destroy();
}
