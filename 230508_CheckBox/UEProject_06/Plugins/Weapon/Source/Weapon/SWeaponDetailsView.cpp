#include "SWeaponDetailsView.h"
#include "SWeaponCheckBoxes.h"
#include "SEquipmentData.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "Weapons/CWeaponAsset.h"
#include "Animation/AnimMontage.h"

TSharedRef<IDetailCustomization> SWeaponDetailsView::MakeInstance()
{
	GLog->Log("MakeInstance");
	return MakeShareable(new SWeaponDetailsView());
}

void SWeaponDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type = UCWeaponAsset::StaticClass();

	DetailBuilder.HideCategory("CWeaponAsset");

	// Attachment
	{
		// 카테고리가 있으면 Return, 없으면 추가함
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Attachment", FText::FromName("Attachment"));
		category.AddProperty("AttachmentClass", type); // 직렬화된 객체의 이름이 있다면 가져와서 넣어줌
	}

	// Equipment
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Equipment", FText::FromName("Equipment"));
		category.AddProperty("EquipmentClass", type);

		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type); // 접혀있는 행, 여기에 EquipmentData의 세부 내용들이 Child로 들어감

		TSharedPtr<SWeaponCheckBoxes> checkBoxes = SEquipmentData::CreateCheckBoxes();
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