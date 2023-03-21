#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeapon.h"
#include "CWeapon_Pistol.generated.h"

UCLASS()
class UEPROJECT_04_API ACWeapon_Pistol : public ACWeapon
{
	GENERATED_BODY()

public:
	ACWeapon_Pistol();

protected:
	void BeginPlay() override;

public:
	void Begin_Equip() override;

	void Begin_Aim() override;
	void End_Aim() override;
	
};
