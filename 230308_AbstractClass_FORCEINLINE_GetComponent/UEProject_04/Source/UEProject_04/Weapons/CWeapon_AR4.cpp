#include "Weapons/CWeapon_AR4.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"

ACWeapon_AR4::ACWeapon_AR4()
{
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'");
	Mesh->SetSkeletalMesh(mesh);

	HolsterSocketName = "Rifle_AR4_Holster";
}
