#include "SWeaponListView.h"
#include "Weapons/CWeaponAsset.h"
#include "EngineUtils.h"
#include "Widgets/Input/SSearchBox.h"

void SWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	Row = InArgs._Row;

	// 여기선 스타일만 셋팅, 실제로 그리는것은 다른 곳
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
	OnListViewSelectedItem = InArgs._OnListViewSelectedItem;

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2, 0)
		[
			SNew(SBox)
			[
				SAssignNew(SearchBox, SSearchBox)
				.SelectAllTextWhenFocused(true) // 검색창에 글씨 선택하면 전체선택되도록 설정
				.OnTextChanged(this, &SWeaponListView::OnTextChanged) // 글자가 바뀔때마다 호출
				.OnTextCommitted(this, &SWeaponListView::OnTextCommitted) // 엔터 눌렀을 때 호출
			]
		]

		+ SVerticalBox::Slot()
		.FillHeight(1) // 화면이 꽉 차게
		[
			// SNew는 Return 값이 없기 때문에 자료형을 지정하여 사용할 수 없다.
			// 자료형을 지정하여 사용하기 위해선 SAssignNew를 사용해야 한다.
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
			.OnGenerateRow(this, &SWeaponListView::OnGenerateRow) // 행을 등록할 때 호출
			.OnSelectionChanged(this, &SWeaponListView::OnSelectionChanged) // 선택이 바뀔때마다 호출
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(FMargin(8, 2))
			[
				SNew(STextBlock)
				.Text(this, &SWeaponListView::OnGetAssetCount) // Slate에서는 Text에 이벤트를 넣어줄 수 있다.
			]
		]
	];

	// 실제 데이터 추가
	ReadDataAssetList();
}

/* Data Asset List 읽어와서 저장하기 */
void SWeaponListView::ReadDataAssetList()
{
	RowDatas.Empty();

	TArray<UObject*> objects;

	// FindOrLoadAssetsByPath : 특정 경로의 모든 애셋을 찾아주는 함수
	// ATL_Class : Class Type Asset
	// ATL_Regular : 객체
	EngineUtils::FindOrLoadAssetsByPath("/Game/Weapons/", objects, EngineUtils::ATL_Regular);

	int32 index = 0;
	for(UObject* obj : objects)
	{
		UCWeaponAsset* asset = Cast<UCWeaponAsset>(obj);

		if (asset == nullptr) continue;

		FString name = asset->GetName();

		if(SearchText.IsEmpty() == false)
		{
			if (name.Contains(SearchText.ToString()) == false)
				continue;
		}

		RowDatas.Add(FWeaponRowData::Make(++index, name, asset));
	}

	// PREDICATE : 정렬조건
	RowDatas.Sort([](const FWeaponRowDataPtr& A, const FWeaponRowDataPtr& B)
	{
		return A->Index < B->Index; // 오름차순 정렬
	});

	ListViewDatas->RequestListRefresh(); // List 갱신
}

FWeaponRowDataPtr SWeaponListView::GetRowDataPtrByName(FString InName)
{
	if (HasRowPtrs() == false)
		return nullptr;

	for (FWeaponRowDataPtr ptr : RowDatas)
	{
		if (ptr->Name == InName)
			return ptr;
	}

	return nullptr;
}

void SWeaponListView::SelectDataPtr(UCWeaponAsset * InAsset)
{
	if (HasRowPtrs() == false)
		return;

	for (FWeaponRowDataPtr ptr : RowDatas)
	{
		if (ptr->Asset == InAsset)
		{
			// 아이템을 바꿔주고 SelectionChange를 Call해줌
			ListViewDatas->SetSelection(ptr); 

			return;
		}
	}
}

FString SWeaponListView::SelectedRowDataPtrName()
{
	TArray<FWeaponRowDataPtr> ptrs = ListViewDatas->GetSelectedItems();
	if (ptrs.Num() > 0)
		return ptrs[0]->Asset->GetName();

	return "";
}

TSharedRef<ITableRow> SWeaponListView::OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable)
{
	// __VA_ARGS__ : 가변형 매개변수
	return SNew(SWeaponTableRow, InTable).Row(InRow); // 그릴 실제 데이터를 넘겨줌
}

void SWeaponListView::OnSelectionChanged(FWeaponRowDataPtr InPtr, ESelectInfo::Type InType)
{
	if (InPtr.IsValid() == false) return;

	//GLog->Log(InPtr->Asset->GetName());

	OnListViewSelectedItem.ExecuteIfBound(InPtr); // If(Bound) 를 Excute에서 알아서 해줌
}

FText SWeaponListView::OnGetAssetCount() const
{
	FString str = FString::Printf(TEXT("%d 에셋"), RowDatas.Num());

	return FText::FromString(str);
}

void SWeaponListView::OnTextChanged(const FText& InText)
{
	if (SearchText.CompareToCaseIgnored(InText) == 0)
		return;

	SearchText = InText; // InText: 입력한 텍스트가 들어옴
	ReadDataAssetList();
}

void SWeaponListView::OnTextCommitted(const FText& InText, ETextCommit::Type InType)
{
	OnTextChanged(InText);
}

