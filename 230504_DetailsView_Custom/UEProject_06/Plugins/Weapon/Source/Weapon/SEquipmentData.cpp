#include "SEquipmentData.h"

#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"

TSharedRef<IPropertyTypeCustomization> SEquipmentData::MakeInstance()
{
	return MakeShareable(new SEquipmentData());
}

// 매개변수 InPropertyHandle은 부모와 같은 객체
void SEquipmentData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow,
	IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	InHeaderRow
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Name"))
	]
	.ValueContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Value"))
	];
}

void SEquipmentData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	uint32 number = 0;
	InPropertyHandle->GetNumChildren(number); // 자식의 개수

	for(uint32 i = 0; i < number; i++)
	{
		TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);

		IDetailPropertyRow& row = InChildBuilder.AddProperty(handle.ToSharedRef());

		row.CustomWidget()
			.NameContent()
			[
				SNew(STextBlock)
				.Text(handle->GetPropertyDisplayName())
		]
		.ValueContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Value"))
		];
	}
}


