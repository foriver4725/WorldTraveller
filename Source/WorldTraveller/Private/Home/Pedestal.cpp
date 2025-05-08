#include "Home/Pedestal.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"

APedestal::APedestal()
{
	collision = this->FindComponentByClass<UShapeComponent>();
}

void APedestal::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(collision))
		collision->OnComponentBeginOverlap.AddDynamic(this, &APedestal::OnBeginOverlap);
}

void APedestal::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Pedestal: %s"), *OtherActor->GetName());
	}
}
