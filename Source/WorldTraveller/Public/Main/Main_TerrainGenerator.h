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

protected:
	virtual void BeginPlay() override final;

private:
	UPROPERTY(EditInstanceOnly, Category = "Property|Data")
	TArray<TObjectPtr<UMaterial>> materials;

	// ノイズのサイズ
	UPROPERTY(EditInstanceOnly, Category = "Property|Value", meta = (ClampMin = "0", ClampMax = "500"))
	uint16 noiseSize = 100;

	// 頂点座標のXY軸スケール倍率
	UPROPERTY(EditInstanceOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float vertexXyScale = 100.0f;

	// 頂点座標の最終スケール倍率
	UPROPERTY(EditInstanceOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float vertexFinalScale = 10.0f;

	// ノイズの周波数スケールの最小値
	UPROPERTY(EditInstanceOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "0.1"))
	float noiseFreqScaleMin = 0.005f;

	// ノイズの周波数スケールの最大値
	UPROPERTY(EditInstanceOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "0.1"))
	float noiseFreqScaleMax = 0.02f;

	// ノイズの高さスケールの最小値
	UPROPERTY(EditInstanceOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float noiseHeightScaleMin = 100.0f;

	// ノイズの高さスケールの最大値
	UPROPERTY(EditInstanceOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float noiseHeightScaleMax = 200.0f;

	UPROPERTY(EditInstanceOnly, Category = "Property|Value")
	FVector meshWorldLocation = FVector::ZeroVector;

	UPROPERTY() TObjectPtr<UProceduralMeshComponent> mesh = nullptr;

	bool hasGeneratedMesh = false;

	void GenerateNewMesh(int32 seed);
};
