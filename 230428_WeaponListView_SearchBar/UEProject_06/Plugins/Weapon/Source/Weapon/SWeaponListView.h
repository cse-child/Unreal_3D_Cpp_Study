#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STableRow.h"

struct FWeaponRowData
{
	int Index;
	FString Name;
	class UCWeaponAsset* Asset;

	FWeaponRowData()
	{
	}

	FWeaponRowData(int32 InIndex, FString InName, class UCWeaponAsset* InAsset)
		:Index(InIndex), Name(InName), Asset(InAsset)
	{
	}

	static TSharedPtr<FWeaponRowData> Make(int32 InIndex, FString InName, class UCWeaponAsset* InAsset)
	{
		return MakeShareable(new FWeaponRowData(InIndex, InName, InAsset));
	}
};

typedef TSharedPtr<FWeaponRowData> FWeaponRowDataPtr;
typedef SListView<FWeaponRowDataPtr> SWeaponListViewRow; // ListView를 Compound에 할당하면 보이게 됨

///////////////////////////////////////////////////////////////////////////////

class WEAPON_API SWeaponTableRow : public SMultiColumnTableRow<FWeaponRowDataPtr>
{
public:
	SLATE_BEGIN_ARGS(SWeaponTableRow) {}
	SLATE_ARGUMENT(FWeaponRowDataPtr, Row)
	SLATE_END_ARGS()

public:
	// S가 붙은 클래스(Slate)는 Construct(FArgument) 함수가 무조건 나와야한다.
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable);

protected:
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;


private:
	FWeaponRowDataPtr Row;
};

///////////////////////////////////////////////////////////////////////////////

DECLARE_DELEGATE_OneParam(FOnWeaponListViewSelectedItem, FWeaponRowDataPtr);

class WEAPON_API SWeaponListView : public SCompoundWidget
{
public:
	// SLATE_ARGUMENT : 변수만 받음
	// SLATE_EVENT : 이벤트만 받음
	// SLATE_ATTRIBUTE : 둘다 받음
	SLATE_BEGIN_ARGS(SWeaponListView){}
	SLATE_EVENT(FOnWeaponListViewSelectedItem, OnListViewSelectedItem);
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	void ReadDataAssetList();

private:
	TSharedRef<ITableRow> OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);
	void OnSelectionChanged(FWeaponRowDataPtr InPtr, ESelectInfo::Type InType);

	FText OnGetAssetCount() const;

	void OnTextChanged(const FText& InText);
	void OnTextCommitted(const FText& InText, ETextCommit::Type InType);

private:
	FOnWeaponListViewSelectedItem OnListViewSelectedItem;

private:
	TArray<FWeaponRowDataPtr> RowDatas;
	TSharedPtr<SWeaponListViewRow> ListViewDatas;

private:
	TSharedPtr<class SSearchBox> SearchBox;
	FText SearchText;
};


