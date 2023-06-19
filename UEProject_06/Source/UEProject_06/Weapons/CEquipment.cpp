#include "Weapons/CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"

void UCEquipment::BeginPlay(ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	Movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	State = CHelpers::GetComponent<UCStateComponent>(InOwner);

}

void UCEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	// Pre
	if (OnEquipmentEquip.IsBound()) // ��Ÿ�� ���� ����Ǿ�� ��
		OnEquipmentEquip.Broadcast();

	if (Data.bCanMove == false)
		Movement->Stop();

	if(!!Data.Montage) // ��Ÿ�ְ� �ִٸ� Begin, End ��Ƽ���̷� ȣ���
	{
		// ing

		// PreBegin (�̺�Ʈ ����ȭ)
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate);
		// PostBegin
	}
	else // ��Ÿ�ְ� ���ٸ� ���Ƿ� Begin, End ȣ��
	{
		Begin_Equip();
		End_Equip();
	}

	if (Data.bUseControlRotation)
		Movement->EnableControlRotation();

	// Post
}

void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;

	if (OnEquipmentBeginEquip.IsBound())
		OnEquipmentBeginEquip.Broadcast();
}

void UCEquipment::End_Equip_Implementation()
{
	// Pre (ȣ�� ����)
	bBeginEquip = false;
	bEquipped = true;

	Movement->Move();
	State->SetIdleMode();

	// Post
	if (OnEquipmentEndEquip.IsBound())
		OnEquipmentEndEquip.Broadcast();
}

void UCEquipment::Unequip_Implementation()
{
	bEquipped = false;
	Movement->DisableControlRotation();

	if (OnEquipmentUnequip.IsBound())
		OnEquipmentUnequip.Broadcast();
}
