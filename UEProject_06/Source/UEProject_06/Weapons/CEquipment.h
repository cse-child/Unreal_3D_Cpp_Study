#pragma once

#include "CoreMinimal.h"
#include "CWeaponStructures.h"
#include "UObject/NoExportTypes.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentEndEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnequip);

UCLASS()
class UEPROJECT_06_API UCEquipment : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE const bool GetBeginEquip() { return bBeginEquip; }
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }

public:
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);

public:
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	void Unequip_Implementation();

public:
	FEquipmentEquip OnEquipmentEquip;
	FEquipmentBeginEquip OnEquipmentBeginEquip;
	FEquipmentEndEquip OnEquipmentEndEquip;
	FEquipmentUnequip OnEquipmentUnequip;

private:
	class UCMovementComponent* Movement;
	class UCStateComponent* State;

private:
	class ACharacter* OwnerCharacter;
	FEquipmentData Data;

private:
	bool bBeginEquip;
	bool bEquipped;
};




