#include "Main/Main_TerrainGenerator.h"
#include "ProceduralMeshComponent.h"
#include "Math/UnrealMathUtility.h"

AMain_TerrainGenerator::AMain_TerrainGenerator()
{
}

TObjectPtr<UProceduralMeshComponent> AMain_TerrainGenerator::CreateNewMesh(const FName& name, const TObjectPtr<UMaterialInterface> material)
{
	TObjectPtr<UProceduralMeshComponent> proceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(name);
	proceduralMesh->SetupAttachment(RootComponent);  // 忘れず親にアタッチするのじゃ

	const int32 sx = -100, ex = 100;
	const int32 sy = -100, ey = 100;
	const int32 width = ex - sx + 1;
	const int32 height = ey - sy + 1;

	// 高さマップを作成するのじゃ
	TArray<TArray<float>> values;
	values.Reserve(height);
	for (int y = sy; y <= ey; ++y)
	{
		TArray<float> xArr;
		xArr.Reserve(width);
		for (int x = sx; x <= ex; ++x)
		{
			float o = FMath::PerlinNoise2D(FVector2D(x * 0.1f, y * 0.1f));
			xArr.Add(o * 50.0f);  // スケーリング（高さ）調整
		}
		values.Add(xArr);
	}

	// 頂点データ
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	for (int32 y = 0; y < height; ++y)
	{
		for (int32 x = 0; x < width; ++x)
		{
			float z = values[y][x];
			Vertices.Add(FVector((x + sx) * 100.0f, (y + sy) * 100.0f, z));  // スケーリングも考慮
			UVs.Add(FVector2D((float)x / width, (float)y / height));
			VertexColors.Add(FLinearColor::White);
			Normals.Add(FVector::UpVector);  // 仮の法線
			Tangents.Add(FProcMeshTangent(1, 0, 0));  // 仮の接線
		}
	}

	// インデックスデータ（トライアングル）
	for (int32 y = 0; y < height - 1; ++y)
	{
		for (int32 x = 0; x < width - 1; ++x)
		{
			int32 i0 = y * width + x;
			int32 i1 = y * width + (x + 1);
			int32 i2 = (y + 1) * width + x;
			int32 i3 = (y + 1) * width + (x + 1);

			Triangles.Add(i0);
			Triangles.Add(i2);
			Triangles.Add(i1);

			Triangles.Add(i1);
			Triangles.Add(i2);
			Triangles.Add(i3);
		}
	}

	// 法線を自前で計算するのじゃ（簡易平均）
	for (int32 i = 0; i < Triangles.Num(); i += 3)
	{
		int32 i0 = Triangles[i];
		int32 i1 = Triangles[i + 1];
		int32 i2 = Triangles[i + 2];

		const FVector& v0 = Vertices[i0];
		const FVector& v1 = Vertices[i1];
		const FVector& v2 = Vertices[i2];

		FVector normal = FVector::CrossProduct(v1 - v0, v2 - v0).GetSafeNormal();

		Normals[i0] += normal;
		Normals[i1] += normal;
		Normals[i2] += normal;
	}
	for (auto& n : Normals)
		n.Normalize();

	proceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
	if (IsValid(material))
		proceduralMesh->SetMaterial(0, material);

	return proceduralMesh;
}
