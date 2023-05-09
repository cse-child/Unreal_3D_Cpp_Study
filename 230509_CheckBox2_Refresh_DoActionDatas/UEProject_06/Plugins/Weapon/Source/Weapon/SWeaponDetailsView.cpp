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
		// 카테고리가 있으면 Return, 없으면 추가함
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Attachment", FText::FromName("Attachment"));
		category.AddProperty("AttachmentClass", type); // 직렬화된 객체의 이름이 있다면 가져와서 넣어줌
	}

	/* Equipment */
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Equipment", FText::FromName("Equipment"));
		category.AddProperty("EquipmentClass", type);

		// 접혀있는 행, 여기에 EquipmentData의 세부 내용들이 Child로 들어감
		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type); 

		if(RefreshByCheckBoxes == false) // Refresh 안된 상태에서만 재생성
		{
			TSharedPtr<SWeaponCheckBoxes> checkBoxes = SEquipmentData::CreateCheckBoxes(); // CheckBox를 제거한 후 다시생성함
			checkBoxes->AddProperties(row.GetPropertyHandle());

			/* 기본값인지 비교하여 찾기 */
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
		//	if (RefreshByCheckBoxes == false) // Refresh 안된 상태에서만 재생성
		//	{
		//		uint32 count = 0;
		//		row.GetPropertyHandle()->GetNumChildren(count);
		//
		//		for(uint32 i = 0; i < count; i++)
		//		{
		//			// 자식의 자식 = 짜식
		//			TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);
		//		}
		//	}// if(RefreshByCheckBoxes)
		//}
	}
}

