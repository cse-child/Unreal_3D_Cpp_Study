#include "Weapons/CWeapon.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/SkeletalMeshComponent.h"

ACWeapon::ACWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);
}

void ACWeapon::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACPlayer>(GetOwner());

	if (HolsterSocketName.IsValid())
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);
}

void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACWeapon::CanEquip()
{
	return true;
}

void ACWeapon::Equip()
{
}

void ACWeapon::Begin_Equip()
{
}

void ACWeapon::End_Equip()
{
}

bool ACWeapon::CanUnequip()
{
	return true;
}

void ACWeapon::Unequip()
{
}


