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
	Super::BeginPlay();

	AttachTo("Holster_Bow");
}

void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();
	
	AttachTo("Hand_Bow_Left");
}

void ACAttachment_Bow::OnUnequip_Implementation()
{
	Super::OnUnequip_Implementation();

	AttachTo("Holster_Bow");
}

