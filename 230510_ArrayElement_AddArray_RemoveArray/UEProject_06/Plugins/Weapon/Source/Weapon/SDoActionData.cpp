#include "SDoActionData.h"
#include "SWeaponCheckBoxes.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"

TArray<TSharedPtr<class SWeaponCheckBoxes>> SDoActionData::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SDoActionData::MakeInstance()
{
	return MakeShareable(new SDoActionData());
}

TSharedPtr<SWeaponCheckBoxes> SDoActionData::AddCheckBoxes()
{
	TSharedPtr<SWeaponCheckBoxes> checkBoxes = MakeShareable(new SWeaponCheckBoxes());
	int32 index = CheckBoxes.Add(checkBoxes);

	return CheckBoxes[index];
}

void SDoActionData::EmptyCheckBoxes()
{
	for (TSharedPtr<SWeaponCheckBoxes> ptr : CheckBoxes)
	{
		if (ptr.IsValid())
			ptr.Reset();
	}

	CheckBoxes.Empty();
}

void SDoActionData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if(CheckBoxes.Num() > 0)
	{
		int32 index = InPropertyHandle->GetIndexInArray(); // 현재 자기자신의 번호
		CheckBoxes[index]->SetUtilities(InCustomizationUtils.GetPropertyUtilities()); // 새로고침을 위한 Utilities
		
		InHeaderRow
		.NameContent()
		[
			InPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
		.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
		[
			CheckBoxes[index]->Draw()
		];

		return;
	}

	/* 기본형 */
	InHeaderRow
	.NameContent()
	[
		InPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
	.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
	[
		InPropertyHandle->CreatePropertyValueWidget()
	];
}

void SDoActionData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InuCustomizationUtils)
{
	if(CheckBoxes.Num() > 0)
	{
		int32 index = InPropertyHandle->GetIndexInArray();
		CheckBoxes[index]->DrawProperties(InPropertyHandle, &InChildBuilder);

		return;
	}

	/* 기본형 */
	uint32 number = 0;
	InPropertyHandle->GetNumChildren(number); // 자식의 개수

	for (uint32 i = 0; i < number; i++)
	{
		TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
		IDetailPropertyRow& row = InChildBuilder.AddProperty(handle.ToSharedRef());
		
		TSharedPtr<SWidget> name;
		TSharedPtr<SWidget> value;
		
		row.GetDefaultWidgets(name, value);
		
		row.CustomWidget()
		.NameContent()
		[
			name.ToSharedRef()
		]
		.ValueContent()
		.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth")) // 넓이 최솟값 고정
		.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
		[
			value.ToSharedRef()
		];
	}
}