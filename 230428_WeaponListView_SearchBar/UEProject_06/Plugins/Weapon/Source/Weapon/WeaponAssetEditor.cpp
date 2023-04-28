#include "WeaponAssetEditor.h"
#include "Weapons/CWeaponAsset.h"
#include "SWeaponListView.h"

const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FWeaponAssetEditor::ListViewTabId = "ListView";

TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;

void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	if(Instance.IsValid())
	{
		Instance->CloseWindow();

		Instance.Reset();
		Instance = nullptr;
	}

	Instance = MakeShareable(new FWeaponAssetEditor());
	Instance->Open(InAssetName);
}

void FWeaponAssetEditor::Shutdown()
{
	if(Instance.IsValid())
	{
		Instance->CloseWindow();

		Instance.Reset();
		Instance = nullptr;
	}
}

void FWeaponAssetEditor::Open(FString InAssetName)
{
	ListView = SNew(SWeaponListView)
		.OnListViewSelectedItem(this, &FWeaponAssetEditor::OnListViewSelectedItem);

	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("WeaponAssetEditor_Layout")
	->AddArea
	(
		/* ToolBar ���� */
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
		)
		/* Tab ���� */
		->Split
		(
			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal) // ���� ���� ����
			->Split
			(
				FTabManager::NewStack() // NewStack�� �ϳ��� ����
				->SetSizeCoefficient(0.175f) // ������ ������ 0.175
				->AddTab(ListViewTabId, ETabState::OpenedTab)
				->SetHideTabWell(true) // ���� ������ ���ϵ��� ����
			)
		)
	);

	// EToolkitMode::Standalone : ������ â
	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, false, true, NewObject<UCWeaponAsset>());
}

void FWeaponAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab tab;
	tab.BindSP(this, &FWeaponAssetEditor::Spawn_ListViewTab); // SP : Slate Pointer
	TabManager->RegisterTabSpawner(ListViewTabId, tab);
}

// SDockTab : Tab �ϳ��� �� ����
TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_ListViewTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
	[
		ListView.ToSharedRef()
	];
}

void FWeaponAssetEditor::OnListViewSelectedItem(FWeaponRowDataPtr InPtr)
{
	if (InPtr == nullptr) return;

	GLog->Log(FString("Editor : ")+InPtr->Asset->GetName());
}

FName FWeaponAssetEditor::GetToolkitFName() const
{
	return EditorName; // ������ â �̸�
}

FText FWeaponAssetEditor::GetBaseToolkitName() const
{
	return FText::FromName(EditorName); // �𸮾󿡼� �ĺ��� �̸�
}

FString FWeaponAssetEditor::GetWorldCentricTabPrefix() const
{
	return EditorName.ToString(); // ȭ�鿡 ��Ÿ�� �̸�
}

FLinearColor FWeaponAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0, 0, 1); // â���� ������ ������ ����
}



