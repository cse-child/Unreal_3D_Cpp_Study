#include "Weapons/CWeaponComponent.h"
#include "Global.h"
#include "CWeapon.h"
#include "Characters/CPlayer.h"
#include "Widgets/CUserWidget_HUD.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetClass<UCUserWidget_HUD>(&HUDClass, "WidgetBlueprint'/Game/Widgets/WB_HUD.WB_HUD_C'");
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

	if(!!HUDClass)
	{
		HUD = CreateWidget<UCUserWidget_HUD, APlayerController>(Owner->GetController<APlayerController>(), HUDClass);
		HUD->AddToViewport();
		HUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(GetCurrentWeapon())
	{
		if (!!HUD)
		{
			GetCurrentWeapon()->IsAutoFire() ? HUD->OnAutoFire() : HUD->OffAutoFire();

			uint8 currCount = GetCurrentWeapon()->GetCurrMagazineCount();
			uint8 maxCount = GetCurrentWeapon()->GetMaxMagazineCount();

			HUD->UpdateMagazine(currCount, maxCount);
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

	if (!!HUD)
		HUD->SetVisibility(ESlateVisibility::Hidden);
}

void UCWeaponComponent::SetAR4Mode()
{
	SetMode(EWeaponType::AR4);
}

void UCWeaponComponent::SetAK47Mode()
{
	SetMode(EWeaponType::AK47);
}

void UCWeaponComponent::SetPistolMode()
{
	//SetMode(EWeaponType::Pistol);
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

	if(!!HUD)
		HUD->SetVisibility(ESlateVisibility::Visible);
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

void UCWeaponComponent::ToggleAutoFire()
{
	CheckNull(GetCurrentWeapon());

	GetCurrentWeapon()->ToggleAutoFire();
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

void UCWeaponComponent::Reload()
{
	CheckNull(GetCurrentWeapon());
	CheckFalse(GetCurrentWeapon()->CanReload());

	GetCurrentWeapon()->Reload();
}

void UCWeaponComponent::Eject_Magazine()
{
	CheckNull(GetCurrentWeapon());

	GetCurrentWeapon()->Eject_Magazine();
}

void UCWeaponComponent::Spawn_Magazine()
{
	CheckNull(GetCurrentWeapon());

	GetCurrentWeapon()->Spawn_Magazine();
}

void UCWeaponComponent::Load_Magazine()
{
	CheckNull(GetCurrentWeapon());

	GetCurrentWeapon()->Load_Magazine();
}

void UCWeaponComponent::End_Reload()
{
	CheckNull(GetCurrentWeapon());

	GetCurrentWeapon()->End_Reload();
}
