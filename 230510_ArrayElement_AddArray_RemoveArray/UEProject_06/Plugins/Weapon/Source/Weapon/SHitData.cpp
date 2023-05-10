#include "SHitData.h"

#include "SWeaponCheckBoxes.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"

TArray<TSharedPtr<class SWeaponCheckBoxes>> SHitData::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SHitData::MakeInstance()
{
	return MakeShareable(new SHitData());
}

TSharedPtr<SWeaponCheckBoxes> SHitData::AddCheckBoxes()
{
	TSharedPtr<SWeaponCheckBoxes> checkBoxes = MakeShareable(new SWeaponCheckBoxes());
	int32 index = CheckBoxes.Add(checkBoxes);

	return CheckBoxes[index];
}

void SHitData::EmptyCheckBoxes()
{
	for (TSharedPtr<SWeaponCheckBoxes> ptr : CheckBoxes)
	{
		if (ptr.IsValid())
			ptr.Reset();
	}

	CheckBoxes.Empty();
}

void SHitData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (CheckBoxes.Num() > 0)
	{
		int32 index = InPropertyHandle->GetIndexInArray();
		CheckBoxes[index]->SetUtilities(InCustomizationUtils.GetPropertyUtilities());

		InHeaderRow
		.NameContent()
		[
			InPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth")) // ³ÐÀÌ ÃÖ¼Ú°ª °íÁ¤
		.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth")) // ³ÐÀÌ ÃÖ´ñ°ª °íÁ¤
		[
			CheckBoxes[index]->Draw()
		];
		return;
	}

	/* ±âº»Çü */
	InHeaderRow
	.NameContent()
	[
		InPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth")) // ³ÐÀÌ ÃÖ¼Ú°ª °íÁ¤
	.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
	[
		InPropertyHandle->CreatePropertyValueWidget()
	];
}

void SHitData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InuCustomizationUtils)
{
	if (CheckBoxes.Num() > 0)
	{
		int32 index = InPropertyHandle->GetIndexInArray();
		CheckBoxes[index]->DrawProperties(InPropertyHandle, &InChildBuilder);

		return;
	}

	/* ±âº»Çü */
	uint32 number = 0;
	InPropertyHandle->GetNumChildren(number); // ÀÚ½ÄÀÇ °³¼ö

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
		.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth")) // ³ÐÀÌ ÃÖ¼Ú°ª °íÁ¤
		.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
		[
			value.ToSharedRef()
		];
	}
}