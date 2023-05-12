#include "SDoActionData.h"
#include "SWeaponCheckBoxes.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "WeaponStyle.h"

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
		int32 index = InPropertyHandle->GetIndexInArray(); // ���� �ڱ��ڽ��� ��ȣ
		CheckBoxes[index]->SetUtilities(InCustomizationUtils.GetPropertyUtilities()); // ���ΰ�ħ�� ���� Utilities

		FString name = InPropertyHandle->GetPropertyDisplayName().ToString();
		name = "DoAction Data - " + name;

		InHeaderRow
		.NameContent()
		[
			SNew(SBorder) // �� ���� ä���
			.BorderImage(FWeaponStyle::Get()->Array_Image.Get())
			[
				InPropertyHandle->CreatePropertyNameWidget(FText::FromString(name))
			]
		]
		.ValueContent()
		.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
		.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
		[
			CheckBoxes[index]->Draw(true)
		];

		return;
	}

	/* �⺻�� */
	InHeaderRow
	.NameContent()
	[
		InPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
	.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
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

	/* �⺻�� */
	uint32 number = 0;
	InPropertyHandle->GetNumChildren(number); // �ڽ��� ����

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
		.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
		.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
		[
			value.ToSharedRef()
		];
	}
}