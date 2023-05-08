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
	InHeaderRow
	.NameContent()
	[
		InPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth")) // 어느정도 값이 줄이들 때 최솟값 고정
	.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
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


