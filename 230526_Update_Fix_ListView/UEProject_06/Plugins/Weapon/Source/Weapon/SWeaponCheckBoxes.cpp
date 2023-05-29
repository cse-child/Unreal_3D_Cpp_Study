#include "SWeaponCheckBoxes.h"
#include "SWeaponDetailsView.h"

#include "Widgets/Layout/SUniformGridPanel.h"
#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "WeaponStyle.h"


void SWeaponCheckBoxes::AddProperties(TSharedPtr<IPropertyHandle> InHandle)
{
	uint32 number = 0;
	InHandle->GetNumChildren(number);

	for (uint32 i = 0; i < number; i++)
		InternalDatas.Add(FInternalData(InHandle->GetChildHandle(i)));
}

void SWeaponCheckBoxes::SetUtilities(TSharedPtr<IPropertyUtilities> InUtilities)
{
	Utilities = InUtilities;
}

TSharedRef<SWidget> SWeaponCheckBoxes::Draw(bool bBackground)
{
	TSharedPtr<SUniformGridPanel> panel;
	SAssignNew(panel, SUniformGridPanel); // panel = SNew() 로 바로 생성해도 동일함

	panel->SetMinDesiredSlotWidth(150);

	for(int32 i = 0; i < InternalDatas.Num(); i++)
	{
		panel->AddSlot(i, 0)
		[
			DrawCheckBox(i)
		];
	}

	if (bBackground == false)
		return panel.ToSharedRef();

	TSharedPtr<SBorder> border = SNew(SBorder) // 행 배경색 채우기
	.BorderImage(FWeaponStyle::Get()->Array_Image.Get())
	[
		panel.ToSharedRef()
	];

	return border.ToSharedRef();
}

void SWeaponCheckBoxes::DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder* InChildBuilder)
{
	for(int32 i = 0; i < InternalDatas.Num(); i++)
	{
		if (InternalDatas[i].bChecked == false) // 기본값이 바뀐 Property만 나타나도록
			continue;

		FString test = InternalDatas[i].bChecked ? "True" : "False"; // True: 기본값에서 바뀜, False: 기본값에서 안바뀜

		TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
		IDetailPropertyRow& row = InChildBuilder->AddProperty(handle.ToSharedRef());

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

/* CheckBox의 내부를 그려주는 함수 */
TSharedRef<SWidget> SWeaponCheckBoxes::DrawCheckBox(int32 InIndex)
{
	return SNew(SCheckBox)
	.IsChecked(InternalDatas[InIndex].bChecked) // 체크박스 체크
	.OnCheckStateChanged(this, &SWeaponCheckBoxes::OnCheckStateChanged, InIndex)
	[
		SNew(STextBlock) // CheckBox 옆에 글씨가 Content 영역
		.Text(FText::FromString(InternalDatas[InIndex].Name))
	];
}

void SWeaponCheckBoxes::CheckDefaultObject(int32 InIndex, UObject* InValue)
{
	UObject* val = nullptr;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (!!val && InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, float InValue)
{
	float val;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, bool InValue)
{
	bool val;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, const FVector& InValue)
{
	FVector val;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::OnCheckStateChanged(ECheckBoxState InState, int InIndex)
{
	InternalDatas[InIndex].bChecked = !InternalDatas[InIndex].bChecked;

	SWeaponDetailsView::OnRefreshByCheckBoxes();
	{
		Utilities->ForceRefresh();
	}
	SWeaponDetailsView::OffRefreshByCheckBoxes();
}

bool SWeaponCheckBoxes::CanDraw(TSharedRef<IPropertyHandle> InHandle, int InCount)
{
	bool bCheck = true;
	bCheck &= InCount > 0;

	int32 index = InHandle->GetIndexInArray();
	bCheck &= index >= 0;
	bCheck &= index < InCount;

	return bCheck;
}