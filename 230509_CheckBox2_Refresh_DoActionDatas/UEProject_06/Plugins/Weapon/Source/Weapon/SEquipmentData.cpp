#include "SEquipmentData.h"

#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SWeaponCheckBoxes.h"

TSharedPtr<class SWeaponCheckBoxes> SEquipmentData::CheckBoxes = nullptr;

TSharedRef<IPropertyTypeCustomization> SEquipmentData::MakeInstance()
{
	return MakeShareable(new SEquipmentData());
}

TSharedPtr<SWeaponCheckBoxes> SEquipmentData::CreateCheckBoxes()
{
	/* CheckBoxes 객체 지우기 */
	if (CheckBoxes.IsValid())
	{
		CheckBoxes.Reset();
		CheckBoxes = nullptr;
	}

	return CheckBoxes = MakeShareable(new SWeaponCheckBoxes());
}


// 매개변수 InPropertyHandle은 부모와 같은 객체
void SEquipmentData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow,
	IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	CheckBoxes->SetUtilities(InCustomizationUtils.GetPropertyUtilities()); // Utilities 넘겨주기
	// Utilities 안에 해당 자료형을 Refresh하라는 함수가 있음

	InHeaderRow
	.NameContent()
	[
		InPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth")) // 넓이 최솟값 고정
	.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth")) // 넓이 최댓값 고정
	[
		CheckBoxes->Draw()
	];
}

void SEquipmentData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	//uint32 number = 0;
	//InPropertyHandle->GetNumChildren(number); // 자식의 개수

	//for(uint32 i = 0; i < number; i++)
	//{
	//	TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
	//	IDetailPropertyRow& row = InChildBuilder.AddProperty(handle.ToSharedRef());

	//	row.CustomWidget()
	//		.NameContent()
	//		[
	//			SNew(STextBlock)
	//			.Text(handle->GetPropertyDisplayName())
	//	]
	//	.ValueContent()
	//	[
	//		SNew(STextBlock)
	//		.Text(FText::FromString("Value"))
	//	];
	//}

	CheckBoxes->DrawProperties(InPropertyHandle, &InChildBuilder);
}


