#include "SWeaponDetailsView.h"
#include "SWeaponCheckBoxes.h"
#include "SEquipmentData.h"
#include "SDoActionData.h"
#include "SHitData.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailPropertyRow.h"
#include "Weapons/CWeaponAsset.h"
#include "Animation/AnimMontage.h"

bool SWeaponDetailsView::RefreshByCheckBoxes = false;

TSharedRef<IDetailCustomization> SWeaponDetailsView::MakeInstance()
{
	return MakeShareable(new SWeaponDetailsView());
}

void SWeaponDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type = UCWeaponAsset::StaticClass();

	//DetailBuilder.HideCategory("CWeaponAsset");

	/* Attachment */
	{
		// 카테고리가 있으면 Return, 없으면 추가함
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("ClassSettings", FText::FromName("Class Settings"));
		category.AddProperty("AttachmentClass", type); // 직렬화된 객체의 이름이 있다면 가져와서 넣어줌
		category.AddProperty("EquipmentClass", type);
		category.AddProperty("DoActionClass", type);
		category.AddProperty("SubActionClass", type);
	}

	/* Equipment */
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Equipment", FText::FromName("Equipment"));

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

	/* Do Action Datas */
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("DoActionData", FText::FromName("DoActionData"));
		IDetailPropertyRow& row = category.AddProperty("DoActionDatas", type);
	
		if (RefreshByCheckBoxes == false) // Refresh 안된 상태에서만 재생성
		{
			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);

			SDoActionData::EmptyCheckBoxes();

			FDoActionData data;
			for(uint32 i = 0; i < count; i++)
			{
				// 자식의 자식 = 짜식
				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);

				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SDoActionData::AddCheckBoxes();
				checkBoxes->AddProperties(handle);

				int32 index = 0;
				checkBoxes->CheckDefaultObject(index++, data.Montage);
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.bCanMove);
				checkBoxes->CheckDefaultValue(index++, data.bFixedCamera);
				checkBoxes->CheckDefaultValue(index++, data.Effect);
				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
			}
		}// if(RefreshByCheckBoxes)
	}

	/* Hit Data */
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("HitData", FText::FromName("HitData"));
		IDetailPropertyRow& row = category.AddProperty("HitDatas", type);

		if (RefreshByCheckBoxes == false) // Refresh 안된 상태에서만 재생성
		{
			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);

			SHitData::EmptyCheckBoxes();

			FHitData data;
			for (uint32 i = 0; i < count; i++)
			{
				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);

				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SHitData::AddCheckBoxes();
				checkBoxes->AddProperties(handle);

				int32 index = 0;
				checkBoxes->CheckDefaultObject(index++, data.Montage);
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.bCanMove);
				checkBoxes->CheckDefaultValue(index++, data.Power);
				checkBoxes->CheckDefaultValue(index++, data.Launch);
				checkBoxes->CheckDefaultValue(index++, data.StopTime);
				checkBoxes->CheckDefaultValue(index++, data.Sound);
				checkBoxes->CheckDefaultValue(index++, data.Effect);
				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
			}
		}// if(RefreshByCheckBoxes)
	}
}

