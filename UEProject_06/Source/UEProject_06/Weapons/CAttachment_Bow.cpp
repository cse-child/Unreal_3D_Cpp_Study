#include "Weapons/CAttachment_Bow.h"
#include "Global.h"
#include "Components/PoseableMeshComponent.h"

ACAttachment_Bow::ACAttachment_Bow()
{
	CHelpers::CreateComponent(this, &Mesh, "Mesh", Root);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");
	Mesh->SetSkeletalMesh(mesh);
}

void ACAttachment_Bow::BeginPlay()
{
	AttachTo("Holster_Bow");

	Super::BeginPlay();
}

void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	CLog::Log("OnBeginEquip_Implementation");
}

void ACAttachment_Bow::OnUnequip_Implementation()
{
	CLog::Log("OnUnequip_Implementation");
}
