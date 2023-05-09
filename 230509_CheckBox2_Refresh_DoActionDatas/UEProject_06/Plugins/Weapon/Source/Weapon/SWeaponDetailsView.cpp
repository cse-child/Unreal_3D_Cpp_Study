#include "SWeaponDetailsView.h"
#include "SWeaponCheckBoxes.h"
#include "SEquipmentData.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailPropertyRow.h"
#include "Weapons/CWeaponAsset.h"
#include "Animation/AnimMontage.h"

bool SWeaponDetailsView::RefreshByCheckBoxes = false;

TSharedRef<IDetailCustomization> SWeaponDetailsView::MakeInstance()
{
	GLog->Log("MakeInstance");
	return MakeShareable(new SWeaponDetailsView());
}

void SWeaponDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type = UCWeaponAsset::StaticClass();

	DetailBuilder.HideCategory("CWeaponAsset");

	/* Attachment */
	{
		// ī�װ��� ������ Return, ������ �߰���
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Attachment", FText::FromName("Attachment"));
		category.AddProperty("AttachmentClass", type); // ����ȭ�� ��ü�� �̸��� �ִٸ� �����ͼ� �־���
	}

	/* Equipment */
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Equipment", FText::FromName("Equipment"));
		category.AddProperty("EquipmentClass", type);

		// �����ִ� ��, ���⿡ EquipmentData�� ���� ������� Child�� ��
		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type); 

		if(RefreshByCheckBoxes == false) // Refresh �ȵ� ���¿����� �����
		{
			TSharedPtr<SWeaponCheckBoxes> checkBoxes = SEquipmentData::CreateCheckBoxes(); // CheckBox�� ������ �� �ٽû�����
			checkBoxes->AddProperties(row.GetPropertyHandle());

			/* �⺻������ ���Ͽ� ã�� */
			FEquipmentData data;

			int32 index = 0;
			checkBoxes->CheckDefaultObject(index++, data.Montage);
			checkBoxes->CheckDefaultValue(index++, data.PlayRate);
			checkBoxes->CheckDefaultValue(index++, data.bCanMove);
			checkBoxes->CheckDefaultValue(index++, data.bUseControlRotation);
		}
	}

	/* Do Action Class */
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("DoAction", FText::FromName("DoAction"));
		IDetailPropertyRow& classRow = category.AddProperty("DoActionClass", type);
		
		TSharedPtr<SWidget> name;
		TSharedPtr<SWidget> value;
		
		classRow.GetDefaultWidgets(name, value);
		
		classRow.CustomWidget()
		.NameContent()
		[
			SNew(STextBlock)
			.ColorAndOpacity(FSlateColor(FLinearColor::Yellow))
			.Text(FText::FromString("DoActionClass"))
		]
		.ValueContent()
		.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
		.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
		[
			value.ToSharedRef()
		];
		
		
		/* Do Action Datas */
		//{
		//	IDetailPropertyRow& row = category.AddProperty("DoActionDatas", type);
		//
		//	if (RefreshByCheckBoxes == false) // Refresh �ȵ� ���¿����� �����
		//	{
		//		uint32 count = 0;
		//		row.GetPropertyHandle()->GetNumChildren(count);
		//
		//		for(uint32 i = 0; i < count; i++)
		//		{
		//			// �ڽ��� �ڽ� = ¥��
		//			TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);
		//		}
		//	}// if(RefreshByCheckBoxes)
		//}
	}
}

