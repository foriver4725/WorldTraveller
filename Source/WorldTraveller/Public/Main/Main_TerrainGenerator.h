#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Main_TerrainGenerator.generated.h"

class UProceduralMeshComponent;

UCLASS()
class WORLDTRAVELLER_API AMain_TerrainGenerator final : public AActor
{
	GENERATED_BODY()

public:
	AMain_TerrainGenerator();

	TObjectPtr<UProceduralMeshComponent> CreateNewMesh(const FName& name, const TObjectPtr<UMaterialInterface> material);
};
