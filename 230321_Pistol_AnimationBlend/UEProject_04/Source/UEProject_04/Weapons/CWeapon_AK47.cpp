#include "Weapons/CWeapon_AK47.h"
#include "Global.h"
#include "CMagazine.h"
#include "CWeaponComponent.h"
#include "Characters/CPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraShake.h"
#include "Widgets/CUserWidget_CrossHair.h"
#include "Animation/AnimMontage.h"

ACWeapon_AK47::ACWeapon_AK47()
{
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'");
	Mesh->SetSkeletalMesh(mesh);

	// ���ذ� (Sight)
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Sight, "Sight", Mesh, "Dotsight");

	UStaticMesh* staticMesh;
	CHelpers::GetAsset<UStaticMesh>(&staticMesh, "StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/SM_T4_Sight.SM_T4_Sight'");
	Sight->SetStaticMesh(staticMesh);
	Sight->SetRelativeScale3D(FVector(1, 0.95f, 1));
	Sight->SetCollisionProfileName("NoCollision");

	// AK
	{
		LeftHandSocketName = "Rifle_AK47_LeftHand";		
	}

	// Equip
	{
		HolsterSocketName = "Rifle_AK47_Holster";
		CHelpers::GetAsset<UAnimMontage>(&EquipMontage, "AnimMontage'/Game/Character/Animations/Rifle_Equip_AK47_Montage.Rifle_Equip_AK47_Montage'");
		EquipMontage_PlayRate = 2;
		RightHandSocketName = "Rifle_AK47_RightHand";
		LeftHandleLocation = FVector(-35, 15.5f, 4);
	}

	// Aim
	{
		BaseData.TargetArmLength = 200;
		BaseData.SocketOffset = FVector(0, 50, 15);
		BaseData.FieldOfView = 90;

		AimData.TargetArmLength = 30;
		AimData.SocketOffset = FVector(-55, 0, 10);
		AimData.FieldOfView = 55;
	}

	// Fire
	{
		RecoilAngle = 0.75f;
		CHelpers::GetClass<UMatineeCameraShake>(&CameraShakeClass, "Blueprint'/Game/Weapons/BP_CameraShake_AK47.BP_CameraShake_AK47_C'");
		AutoFireInterval = 0.15f;
		SpreadSpeed = 2.0f;
		MaxSpreadAlignment = 2.0f;
	}

	// UI
	{
		CHelpers::GetClass<UCUserWidget_CrossHair>(&CrossHairClass, "WidgetBlueprint'/Game/Widgets/WB_CrossHair.WB_CrossHair_C'");
	}

	// Magazine
	{
		MaxMagazineCount = 30;
		CHelpers::GetAsset<UAnimMontage>(&ReloadMontage, "AnimMontage'/Game/Character/Animations/Rifle_Reload_Montage.Rifle_Reload_Montage'");
		ReloadMontage_PlayRate = 1.5f;
		MagazineBoneName = "b_gun_mag";
		CHelpers::GetClass<ACMagazine>(&MagazineClass, "Blueprint'/Game/Weapons/BP_CMagazine_AK47.BP_CMagazine_AK47_C'");
		MagazineSocketName = "Rifle_Magazine";
	}

	// ArmsBlueprint'/Game/Weapons/BP_CMagazine_AK47.BP_CMagazine_AK47'
	{
		ArmsMeshTransform.SetLocation(FVector(-14.25f, -5.85f, -156.935f));
		ArmsMeshTransform.SetRotation(FQuat(FRotator(-0.5f, -11.85f, -1.2f)));

		ArmsLeftHandTransform.SetLocation(FVector(-33, 11, -1.5f));
		ArmsLeftHandTransform.SetRotation(FQuat(FRotator(-4, -138, 77)));
	}
}

void ACWeapon_AK47::Begin_Equip()
{
	if (LeftHandSocketName.IsValid())
		CHelpers::AttachTo(this, Owner->GetMesh(), LeftHandSocketName);

	Owner->GetArms()->SetRelativeTransform(ArmsMeshTransform);
}

void ACWeapon_AK47::End_Equip()
{
	Super::Begin_Equip();
	Super::End_Equip();
}

void ACWeapon_AK47::Begin_Aim()
{
	Super::Begin_Aim();

	if (!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Hidden);

	Owner->GetMesh()->SetVisibility(false);
	Owner->GetBackpack()->SetVisibility(false);
	Owner->GetArms()->SetVisibility(true);

	CHelpers::AttachTo(this, Owner->GetArms(), RightHandSocketName);

	CHelpers::GetComponent<UCWeaponComponent>(Owner)->OnWeaponAim_Arms_Begin.Broadcast(this);
}

void ACWeapon_AK47::End_Aim()
{
	Super::End_Aim();

	if (!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Visible);

	Owner->GetMesh()->SetVisibility(true);
	Owner->GetBackpack()->SetVisibility(true);
	Owner->GetArms()->SetVisibility(false);

	CHelpers::AttachTo(this, Owner->GetMesh(), RightHandSocketName);

	CHelpers::GetComponent<UCWeaponComponent>(Owner)->OnWeaponAim_Arms_End.Broadcast();
}