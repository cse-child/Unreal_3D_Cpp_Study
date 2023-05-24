#include "SEquipmentData.h"
#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SWeaponCheckBoxes.h"
#include "WeaponStyle.h"

TSharedPtr<class SWeaponCheckBoxes> SEquipmentData::CheckBoxes = nullptr;

TSharedRef<IPropertyTypeCustomization> SEquipmentData::MakeInstance()
{
	return MakeShareable(new SEquipmentData());
}

TSharedPtr<SWeaponCheckBoxes> SEquipmentData::CreateCheckBoxes()
{
	/* CheckBoxes ��ü ����� */
	if (CheckBoxes.IsValid())
	{
		CheckBoxes.Reset();
		CheckBoxes = nullptr;
	}

	return CheckBoxes = MakeShareable(new SWeaponCheckBoxes());
}


// �Ű����� InPropertyHandle�� �θ�� ���� ��ü
void SEquipmentData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow,
	IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	// üũ�ڽ��� �������� �ʴ� ��� �⺻������ ���
	if(CheckBoxes.IsValid() == false) 
	{
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
		
		return;
	}

	CheckBoxes->SetUtilities(InCustomizationUtils.GetPropertyUtilities()); // Utilities �Ѱ��ֱ�
	// Utilities �ȿ� �ش� �ڷ����� Refresh�϶�� �Լ��� ����

	InHeaderRow
	.NameContent()
	[
		InPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
	.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
	[
		CheckBoxes->Draw()
	];
}

void SEquipmentData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if(CheckBoxes.IsValid() == false)
	{
		/* �⺻�� */
		uint32 number = 0;
		InPropertyHandle->GetNumChildren(number); // �ڽ��� ����

		for(uint32 i = 0; i < number; i++)
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
		} // for(i)
		return;
	}
	CheckBoxes->DrawProperties(InPropertyHandle, &InChildBuilder);
}


