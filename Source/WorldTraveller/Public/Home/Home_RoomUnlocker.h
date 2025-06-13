#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Home_RoomUnlocker.generated.h"

class AStaticMeshActor;

UCLASS()
class WORLDTRAVELLER_API AHome_RoomUnlocker final : public AActor
{
	GENERATED_BODY()

public:
	AHome_RoomUnlocker();

protected:
	virtual void Tick(float DeltaTime) override final;

private:
	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<AStaticMeshActor> northWall;

	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<AStaticMeshActor> eastWall;

	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<AStaticMeshActor> southWall;

	UPROPERTY(EditInstanceOnly, Category = "Property|Dependency")
	TObjectPtr<AStaticMeshActor> westWall;

	void UnlockNorthRoom();  // プレイ記録展示部屋
	void UnlockEastRoom();  // 使用用途未定
	void UnlockSouthRoom();  // 使用用途未定
	void UnlockWestRoom();  // 使用用途未定

	bool bFirstTick = true;
};
