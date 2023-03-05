#include "01_Spawn/C02_Mesh.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"

AC02_Mesh::AC02_Mesh()
{
	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//RootComponent = Mesh;

	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh");

	//ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(L"StaticMesh'/Game/Meshes/Cube.Cube'");
	//if (mesh.Succeeded())
	//	Mesh->SetStaticMesh(mesh.Object);

	UStaticMesh* mesh = nullptr;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Meshes/Cube.Cube'");
	Mesh->SetStaticMesh(mesh);
}

void AC02_Mesh::BeginPlay()
{
	Super::BeginPlay();
	
}


