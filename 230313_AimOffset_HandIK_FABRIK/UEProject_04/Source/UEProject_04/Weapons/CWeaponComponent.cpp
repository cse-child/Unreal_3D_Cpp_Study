#include "Weapons/CWeaponComponent.h"
#include "Global.h"
#include "CWeapon.h"
#include "Characters/CPlayer.h"

UCWeaponComponent::UCWeaponComponent()
{

}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACPlayer>(GetOwner());
	CheckNull(Owner);

	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for(TSubclassOf<ACWeapon> weaponClass : WeaponClasses)
	{
		if(!!weaponClass)
		{
			ACWeapon* weapon = Owner->GetWorld()->SpawnActor<ACWeapon>(weaponClass, params);
			Weapons.Add(weapon);
		}
	}
}

ACWeapon* UCWeaponComponent::GetCurrentWeapon()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);

	return Weapons[(int32)Type];
}

void UCWeaponComponent::SetUnarmedMode()
{
	CheckFalse(GetCurrentWeapon()->CanUnequip());

	GetCurrentWeapon()->Unequip();
	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetAR4Mode()
{
	SetMode(EWeaponType::AR4);
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if(Type == InType)
	{
		SetUnarmedMode();

		return;
	}
	else if(IsUnarmedMode() == false)
	{
		CheckFalse(GetCurrentWeapon()->CanUnequip());

		GetCurrentWeapon()->Unequip();
	}

	CheckNull(Weapons[(int32)InType]);

	CheckFalse(Weapons[(int32)InType]->CanEquip());
	Weapons[(int32)InType]->Equip();

	ChangeType(InType);
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = InType;
	Type = InType;

	if (OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(prevType, InType);
}

void UCWeaponComponent::Begin_Equip()
{
	CheckNull(GetCurrentWeapon());

	GetCurrentWeapon()->Begin_Equip();
}

void UCWeaponComponent::End_Equip()
{
	CheckNull(GetCurrentWeapon());

	GetCurrentWeapon()->End_Equip();
}

void UCWeaponComponent::Begin_Aim()
{
	CheckNull(GetCurrentWeapon());
	CheckFalse(GetCurrentWeapon()->CanAim());

	GetCurrentWeapon()->Begin_Aim();
}

void UCWeaponComponent::End_Aim()
{
	CheckNull(GetCurrentWeapon());

	GetCurrentWeapon()->End_Aim();
}

void UCWeaponComponent::Begin_Fire()
{
	CheckNull(GetCurrentWeapon());
	CheckFalse(GetCurrentWeapon()->CanFire());

	GetCurrentWeapon()->Begin_Fire();
}

void UCWeaponComponent::End_Fire()
{
	CheckNull(GetCurrentWeapon());

	GetCurrentWeapon()->End_Fire();
}

bool UCWeaponComponent::IsInAim()
{
	CheckNullResult(GetCurrentWeapon(), false);

	return GetCurrentWeapon()->IsInAim();
}

FVector UCWeaponComponent::GetLeftHandLocation()
{
	CheckNullResult(GetCurrentWeapon(), FVector::ZeroVector);

	return GetCurrentWeapon()->GetLeftHandLocation();
}
