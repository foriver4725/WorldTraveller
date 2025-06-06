#include "Main/Main_TerrainGenerator.h"
#include "ProceduralMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Math/RandomStream.h"
#include "WorldTravellerGameInstance.h"

AMain_TerrainGenerator::AMain_TerrainGenerator()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));
	SetRootComponent(mesh);
}

void AMain_TerrainGenerator::GenerateRandomTerrain(const FRandomStream& rand)
{
	if (hasGeneratedMesh) return;
	hasGeneratedMesh |= true;

	// パラメータの計算
	const uint16 Width = noiseSize, Height = noiseSize;
	// ランダムな開始位置 (オーバーフロー防止で、2bit分猶予を持たせる)
	const int32 StartX = rand.RandRange(0, MAX_int32 >> 2), StartY = rand.RandRange(0, MAX_int32 >> 2);
	const int32 EndX = StartX + Width - 1, EndY = StartY + Height - 1;

	// 高さマップ生成（2Dパーリンノイズ）
	TArray<TArray<float>> HeightMap;
	HeightMap.Reserve(Height);
	for (int32 Y = StartY; Y <= EndY; ++Y)
	{
		TArray<float> Row;
		Row.Reserve(Width);
		for (int32 X = StartX; X <= EndX; ++X)
		{
			float NoiseFreqScale = rand.FRandRange(noiseFreqScaleMin, noiseFreqScaleMax); // ノイズの周波数スケールをランダムに設定
			float NoiseHeightScale = rand.FRandRange(noiseHeightScaleMin, noiseHeightScaleMax); // ノイズの高さスケールをランダムに設定
			float NoiseValue = FMath::PerlinNoise2D(FVector2D(X, Y) * NoiseFreqScale);
			Row.Add(NoiseValue * NoiseHeightScale); // スケーリングして高さを増幅
		}
		HeightMap.Add(Row);
	}

	// メッシュデータ初期化
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	Vertices.Reserve(Width * Height);
	Normals.Reserve(Width * Height);
	UVs.Reserve(Width * Height);
	VertexColors.Reserve(Width * Height);
	Tangents.Reserve(Width * Height);

	// 頂点データ作成
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			float Z = HeightMap[Y][X];
			Vertices.Add(FVector((X - (Width >> 1)) * vertexXyScale, (Y - (Height >> 1)) * vertexXyScale, Z) * vertexFinalScale);
			UVs.Add(FVector2D((float)X / (Width - 1), (float)Y / (Height - 1)) * uvScale);
			VertexColors.Add(FLinearColor::White);
			Normals.Add(FVector::ZeroVector); // 仮値。あとで計算
			Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f)); // 仮の接線
		}
	}

	// 三角形インデックスを作成（2枚の三角形で1つの四角形を構成）
	for (int32 Y = 0; Y < Height - 1; ++Y)
	{
		for (int32 X = 0; X < Width - 1; ++X)
		{
			int32 I0 = Y * Width + X;
			int32 I1 = Y * Width + (X + 1);
			int32 I2 = (Y + 1) * Width + X;
			int32 I3 = (Y + 1) * Width + (X + 1);

			Triangles.Add(I0);
			Triangles.Add(I2);
			Triangles.Add(I1);

			Triangles.Add(I1);
			Triangles.Add(I2);
			Triangles.Add(I3);
		}
	}

	// 法線を各三角形の面法線の加算で算出
	for (int32 i = 0; i < Triangles.Num(); i += 3)
	{
		int32 Index0 = Triangles[i];
		int32 Index1 = Triangles[i + 1];
		int32 Index2 = Triangles[i + 2];

		const FVector& V0 = Vertices[Index0];
		const FVector& V1 = Vertices[Index1];
		const FVector& V2 = Vertices[Index2];

		FVector FaceNormal = FVector::CrossProduct(V2 - V0, V1 - V0).GetSafeNormal();
		Normals[Index0] += FaceNormal;
		Normals[Index1] += FaceNormal;
		Normals[Index2] += FaceNormal;
	}

	// 単位ベクトルに正規化
	for (FVector& N : Normals)
	{
		N.Normalize();
	}

	// メッシュセクション作成
	mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
	mesh->bUseAsyncCooking = true;

	// マテリアル設定
	if (materials.Num() > 0)
	{
		const TObjectPtr<UMaterial> material = materials[rand.RandRange(0, materials.Num() - 1)];
		if (IsValid(material))
			mesh->SetMaterial(0, material);
	}

	// メッシュの位置を設定
	mesh->SetWorldLocation(meshWorldLocation);
}
