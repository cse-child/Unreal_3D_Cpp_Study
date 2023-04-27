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
	// S가 붙은 클래스(Slate)는 Construct(FArgument) 함수가 무조건 나온다.
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable);

protected:
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;


private:
	FWeaponRowDataPtr Row;
};

///////////////////////////////////////////////////////////////////////////////

class WEAPON_API SWeaponListView : public SCompoundWidget
{
public:
	// 외부의 Slate 문법에서 Argument를 넘겨줄 때 사용하는 문법
	// 외부에서는 _ 를 사용해서 접근한다.
	SLATE_BEGIN_ARGS(SWeaponListView){}
	SLATE_ARGUMENT(FString, Text)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	TSharedRef<ITableRow> OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);

private:
	TArray<FWeaponRowDataPtr> RowDatas;
	TSharedPtr<SWeaponListViewRow> ListViewDatas;
};
