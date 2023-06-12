#include "01_Spawn/C02_Mesh_Cone.h"
#include "Global.h"

AC02_Mesh_Cone::AC02_Mesh_Cone()
{
	UStaticMesh* mesh = nullptr;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Meshes/Cone.Cone'");
	Mesh->SetStaticMesh(mesh);
}