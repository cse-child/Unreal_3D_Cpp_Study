#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CDoAction.h"
#include "CEquipment.h"
#include "CSubAction.h"
#include "CWeaponData.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
	SubActionClass = UCSubAction::StaticClass();
}

void UCWeaponAsset::BeginPlay(ACharacter* InOwner, UCWeaponData** OutWeaponData)
{
	ACAttachment* attachment = nullptr;  // ������ ��ü�� ������ ��, WeaponData�� �ʱ�ȭ
	if(!!AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	UCEquipment* equipment = nullptr;
	if(!!EquipmentClass)
	{
		equipment = NewObject<UCEquipment>(this, EquipmentClass);
		equipment->BeginPlay(InOwner, EquipmentData);

		if(!!attachment)
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(attachment, &ACAttachment::OnBeginEquip);
			equipment->OnEquipmentUnequip.AddDynamic(attachment, &ACAttachment::OnUnequip);
		}
	}

	UCDoAction* doAction = nullptr;
	if(!!DoActionClass)
	{
		doAction = NewObject<UCDoAction>(this, DoActionClass);
		doAction->BeginPlay(attachment, equipment, InOwner, DoActionDatas, HitDatas);

		if (!!attachment)
		{
			attachment->OnAttachmentBeginCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginCollision);
			attachment->OnAttachmentEndCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentEndCollision);

			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentEndOverlap);
		}

		if(!!equipment)
		{
			equipment->OnEquipmentEquip.AddDynamic(doAction, &UCDoAction::OnEquip);
			equipment->OnEquipmentBeginEquip.AddDynamic(doAction, &UCDoAction::OnBeginEquip);
			equipment->OnEquipmentEndEquip.AddDynamic(doAction, &UCDoAction::OnEndEquip);
			equipment->OnEquipmentUnequip.AddDynamic(doAction, &UCDoAction::OnUnequip);
		}
	}

	UCSubAction* subAction = nullptr;
	if(!!SubActionClass)
	{
		subAction = NewObject<UCSubAction>(this, SubActionClass);
		subAction->BeginPlay(InOwner, attachment, doAction);
	}

	/* Weapon Data ���� */
	*OutWeaponData = NewObject<UCWeaponData>();
	(*OutWeaponData)->Attachment = attachment;
	(*OutWeaponData)->Equipment = equipment;
	(*OutWeaponData)->DoAction = doAction;
	(*OutWeaponData)->SubAction = subAction;
}

#if WITH_EDITOR
void UCWeaponAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	CheckTrue(FApp::IsGame());

	// PropertyChangedEvent.GetPropertyName() : ����� �׸��� Property �̸� ��ȯ
	bool bRefresh = false;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare("DoActionDatas") == 0; // Compare == 0 : ����
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare("HitDatas") == 0;

	if (bRefresh)
	{
		bool bCheck = false;
		/* �迭 ���� ���´� 4���� */
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd;		//�߰�
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove;	//����
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear;	//õ������
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate;	//����

		if (bCheck)
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			TSharedPtr<IDetailsView> detailsView = prop.FindDetailView("WeaponAssetEditorDetailsView");

			if (detailsView.IsValid())
				detailsView->ForceRefresh(); // ����
		}
	}
}
#endif //WITH_EDITOR




