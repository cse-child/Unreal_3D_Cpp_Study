#include "SWeaponListView.h"
#include "Weapons/CWeaponAsset.h"


void SWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	Row = InArgs._Row;

	// ���⼱ ��Ÿ�ϸ� ����, ������ �׸��°��� �ٸ���
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
		.FillHeight(1) // ȭ���� �� ����
		[
			// SNew�� Return ���� ���� ������ �ڷ����� �����Ͽ� ����� �� ����.
			//SNew(STextBlock)
			//.Text(FText::FromString(InArgs._Text))

			// �ڷ����� �����Ͽ� ����ϱ� ���ؼ� SAssignNew�� ���
			SAssignNew(ListViewDatas, SWeaponListViewRow)
			.HeaderRow // ��� �ʼ�
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column("Index") // �𸮾��� �ĺ��� �̸�
				.DefaultLabel(FText::FromString("")) // ��µ� �̸�
				.ManualWidth(50)
				+ SHeaderRow::Column("Name")
				.DefaultLabel(FText::FromString("Name"))
			)
			.ListItemsSource(&RowDatas) // ����Ʈ���� ������ ������
			.OnGenerateRow(this, &SWeaponListView::OnGenerateRow)
		]
	];

	RowDatas.Add(FWeaponRowData::Make(1, "Test1", nullptr));
	RowDatas.Add(FWeaponRowData::Make(2, "Test2", nullptr));
	RowDatas.Add(FWeaponRowData::Make(3, "Test3", nullptr));
	RowDatas.Add(FWeaponRowData::Make(4, "Test4", nullptr));

	ListViewDatas->RequestListRefresh(); // List ����
}

TSharedRef<ITableRow> SWeaponListView::OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable)
{
	// __VA_ARGS__ : ������ �Ű�����
	return SNew(SWeaponTableRow, InTable)
		.Row(InRow); // �׸� ���� �����͸� �Ѱ���
}

