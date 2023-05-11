#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CDoAction.h"
#include "CEquipment.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();

}

void UCWeaponAsset::BeginPlay(ACharacter* InOwner)
{
	if(!!AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		Attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	if(!!EquipmentClass)
	{
		Equipment = NewObject<UCEquipment>(this, EquipmentClass);
		Equipment->BeginPlay(InOwner, EquipmentData);

		if(!!Attachment)
		{
			Equipment->OnEquipmentBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
			Equipment->OnEquipmentUnequip.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}

	if(!!DoActionClass)
	{
		DoAction = NewObject<UCDoAction>(this, DoActionClass);
		DoAction->BeginPlay(Attachment, Equipment, InOwner, DoActionDatas, HitDatas);

		if (!!Attachment)
		{
			Attachment->OnAttachmentBeginCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginCollision);
			Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);

			Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndOverlap);

		}
	}
}

#if WITH_EDITOR
void UCWeaponAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	CheckTrue(FApp::IsGame());

	// PropertyChangedEvent.GetPropertyName() : 변경된 항목의 Property 이름 반환
	bool bRefresh = false;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare("DoActionDatas") == 0; // Compare == 0 : 같다
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare("HitDatas") == 0;

	if (bRefresh)
	{
		bool bCheck = false;
		/* 배열 관련 상태는 4가지 */
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd;		//추가
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove;	//삭제
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear;	//천제삭제
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate;	//복제

		if (bCheck)
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			TSharedPtr<IDetailsView> detailsView = prop.FindDetailView("WeaponAssetEditorDetailsView");

			if (detailsView.IsValid())
				detailsView->ForceRefresh(); // 갱신
		}
	}
}
#endif //WITH_EDITOR




