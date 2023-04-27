#include "SWeaponListView.h"
#include "Weapons/CWeaponAsset.h"


void SWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	Row = InArgs._Row;

	// 여기선 스타일만 셋팅, 실제로 그리는것은 다른곳
	SMultiColumnTableRow<FWeaponRowDataPtr>::Construct
	(
		FSuperRowType::FArguments().Style(FEditorStyle::Get(), "TableView.DarkRow"), InOwnerTable
	);
}

TSharedRef<SWidget> SWeaponTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	FString str;
	if (InColumnName == "Index")
		str = FString::FromInt(Row->Index);
	else if (InColumnName == "Name")
		str = Row->Name;

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(4)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::FromString(str))
		];
}

///////////////////////////////////////////////////////////////////////////////

void SWeaponListView::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1) // 화면이 꽉 차게
		[
			// SNew는 Return 값이 없기 때문에 자료형을 지정하여 사용할 수 없다.
			//SNew(STextBlock)
			//.Text(FText::FromString(InArgs._Text))

			// 자료형을 지정하여 사용하기 위해선 SAssignNew를 사용
			SAssignNew(ListViewDatas, SWeaponListViewRow)
			.HeaderRow // 헤더 필수
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column("Index") // 언리얼이 식별할 이름
				.DefaultLabel(FText::FromString("")) // 출력될 이름
				.ManualWidth(50)
				+ SHeaderRow::Column("Name")
				.DefaultLabel(FText::FromString("Name"))
			)
			.ListItemsSource(&RowDatas) // 리스트에서 보여줄 아이템
			.OnGenerateRow(this, &SWeaponListView::OnGenerateRow)
		]
	];

	RowDatas.Add(FWeaponRowData::Make(1, "Test1", nullptr));
	RowDatas.Add(FWeaponRowData::Make(2, "Test2", nullptr));
	RowDatas.Add(FWeaponRowData::Make(3, "Test3", nullptr));
	RowDatas.Add(FWeaponRowData::Make(4, "Test4", nullptr));

	ListViewDatas->RequestListRefresh(); // List 갱신
}

TSharedRef<ITableRow> SWeaponListView::OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable)
{
	// __VA_ARGS__ : 가변형 매개변수
	return SNew(SWeaponTableRow, InTable)
		.Row(InRow); // 그릴 실제 데이터를 넘겨줌
}

