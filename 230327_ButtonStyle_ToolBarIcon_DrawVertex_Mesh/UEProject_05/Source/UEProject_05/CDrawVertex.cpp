#include "CDrawVertex.h"
#include "Global.h"
#include "ProceduralMeshComponent.h"

ACDrawVertex::ACDrawVertex()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;

	CHelpers::CreateComponent<UProceduralMeshComponent>(this, &Mesh, "Mesh");

	// 중앙을 0으로 잡기 위함
	FVector p = FVector(0.5f, 0.5f, 0.5f);

	// 정점을 그리는 방향: 반시계
	// Front
	{
		Positions.Add(FVector(-p.X, -p.Y, -p.Z)); // 0
		Positions.Add(FVector(-p.X, -p.Y, +p.Z)); // 1
		Positions.Add(FVector(-p.X, +p.Y, -p.Z)); // 2
		Positions.Add(FVector(-p.X, +p.Y, +p.Z)); // 3

		for (int32 i = 0; i < 4; i++)
		{
			Colors.Add(FColor(128, 0, 0, 255));
			Normals.Add(FVector(-1, 0, 0));
		}
		AddTriangles(0);
	}

	// Back
	{
		Positions.Add(FVector(+p.X, +p.Y, -p.Z));
		Positions.Add(FVector(+p.X, +p.Y, +p.Z));
		Positions.Add(FVector(+p.X, -p.Y, -p.Z));
		Positions.Add(FVector(+p.X, -p.Y, +p.Z));

		for (int32 i = 0; i < 4; i++)
		{
			Colors.Add(FColor(0, 128, 0, 255));
			Normals.Add(FVector(+1, 0, 0));
		}
		AddTriangles(4);
	}

	// Top
	{
		Positions.Add(FVector(-p.X, -p.Y, +p.Z));
		Positions.Add(FVector(+p.X, -p.Y, +p.Z));
		Positions.Add(FVector(-p.X, +p.Y, +p.Z));
		Positions.Add(FVector(+p.X, +p.Y, +p.Z));

		for (int32 i = 0; i < 4; i++)
		{
			Colors.Add(FColor(0, 0, 128, 255));
			Normals.Add(FVector(0, 0, +1));
		}
		AddTriangles(8);
	}

	// Bottom
	{
		Positions.Add(FVector(-p.X, -p.Y, -p.Z));
		Positions.Add(FVector(-p.X, +p.Y, -p.Z));
		Positions.Add(FVector(+p.X, -p.Y, -p.Z));
		Positions.Add(FVector(+p.X, +p.Y, -p.Z));

		for (int32 i = 0; i < 4; i++)
		{
			Colors.Add(FColor(0, 128, 128, 255));
			Normals.Add(FVector(0, 0, -1));
		}
		AddTriangles(12);
	}

	// Left
	{
		Positions.Add(FVector(+p.X, -p.Y, -p.Z));
		Positions.Add(FVector(+p.X, -p.Y, +p.Z));
		Positions.Add(FVector(-p.X, -p.Y, -p.Z));
		Positions.Add(FVector(-p.X, -p.Y, +p.Z));

		for (int32 i = 0; i < 4; i++)
		{
			Colors.Add(FColor(128, 0, 128, 255));
			Normals.Add(FVector(0, -1, 0));
		}
		AddTriangles(16);
	}

	// Right
	{
		Positions.Add(FVector(-p.X, +p.Y, -p.Z));
		Positions.Add(FVector(-p.X, +p.Y, +p.Z));
		Positions.Add(FVector(+p.X, +p.Y, -p.Z));
		Positions.Add(FVector(+p.X, +p.Y, +p.Z));

		for (int32 i = 0; i < 4; i++)
		{
			Colors.Add(FColor(128, 128, 128, 255));
			Normals.Add(FVector(0, +1, 0));
		}
		AddTriangles(20);
	}

	/* Uvs는 공통
	0,0 ㅡㅡ 1,0
	|          |
	|          |
	|          |
	0,1 ㅡㅡ 1,1
	*/
	for(int32 i = 0; i < 6; i++) // 6 = Front, Back, Top, Bottom, Left, Right
	{
		Uvs.Add(FVector2D(0, 1));
		Uvs.Add(FVector2D(0, 0));
		Uvs.Add(FVector2D(1, 1));
		Uvs.Add(FVector2D(1, 0));
	}

	// Create Mesh
	Mesh->CreateMeshSection(0, Positions, Indices, Normals, Uvs, Colors, TArray<FProcMeshTangent>(), true);
	Mesh->SetRelativeScale3D(FVector(100));
}

void ACDrawVertex::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACDrawVertex::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/* 삼각형을 그릴땐 반시계방향으로 그린다.
	1 ㅡㅡㅡ 3
	|       |
	|       |
	0 ㅡㅡㅡ 2
 */
void ACDrawVertex::AddTriangles(int32 InStart)
{
	// 시계방향 : 0 1 2
	// 반시계방향 : 2 1 0 = 0 2 1 = 1 0 2 시작지점은 중요하지 않다.
	Indices.Add(InStart + 2);
	Indices.Add(InStart + 1);
	Indices.Add(InStart + 0);

	// 시계방향 : 2 1 3
	// 반시계방향 : 3 1 2
	Indices.Add(InStart + 3);
	Indices.Add(InStart + 1);
	Indices.Add(InStart + 2);
}