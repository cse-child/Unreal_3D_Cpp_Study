#include "01_Spawn/C02_Mesh_Sphere.h"
#include "Global.h"

AC02_Mesh_Sphere::AC02_Mesh_Sphere()
{
	UStaticMesh* mesh = nullptr;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Meshes/Sphere.Sphere'");
	Mesh->SetStaticMesh(mesh);
}