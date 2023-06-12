#include "WeaponAssetEditor.h"
#include "Weapons/CWeaponAsset.h"
#include "SWeaponLeftArea.h"
#include "SWeaponDetailsView.h"

const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FWeaponAssetEditor::LeftAreaTabId = "LeftArea";
const FName FWeaponAssetEditor::DetailTabId = "Details";

TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;

void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	if(Instance.IsValid())
	{
		// �̹� ������ â�� �����ִ� ���¿��� �ٸ� DA Ŭ������ ������
		// -> ���� �׸����� ���� ������ â�� �����Ų��.
		// �̺κ� �ڵ尡 ������ �ٸ� DAŬ���� �� �� â�� ���������� ��
		if (Instance->LeftArea.IsValid())
		{
			FWeaponRowDataPtr ptr = nullptr;
			
			if (InAssetName.Len() > 0)
				ptr = Instance->LeftArea->GetRowDataPtrByName(InAssetName);

			if (ptr == nullptr)
				ptr = Instance->LeftArea->GetFirstDataPtr();

			Instance->LeftArea->SelectDataPtr(ptr->Asset);

			return;
		}

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
	/* ListView ��ü ���� */
	LeftArea = SNew(SWeaponLeftArea).OnListViewSelectedItem(this, &FWeaponAssetEditor::OnListViewSelectedItem);

	/* Detail Tab ��ü ���� */
	// FPropertyEditorModule : ������ �����ϴ� ��� Detail View�� �����ϴ� ���
	FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	{
	/*
	FDetailsViewArgs �Ű�����
	
	const bool InUpdateFromSelection			: �����Ҷ����� Update
	const bool InLockable						: ������ �� ������ ��ױ�
	const bool InAllowSearch					: ������(ã��) ����� ��뿩��
	const ENameAreaSettings InNameAreaSettings
	-> ActorsUserNameArea						: Actor�� �̸��� ���
	-> ComponentsAndActorsUseNameArea			: Component�� Actor�� �̸� ���
	-> ObjectsUseNameArea						: Object�� �̸� ���
	const bool InHideSelectionTip				: ������ ������Ʈ�� ���� �� Tip�� ǥ��
	FNotifyHook* InNotifyHook					: �Ӽ��� ����Ǹ� ȣ���ϴ� Hook
	const bool InSearchInitialKeyFocus			: �˻�â�� �ʱ� Ű���� ��Ŀ���� �α�
	FName InViewIdentifier						: ����� �ĺ���
	*/
	}
	FDetailsViewArgs args(false, false, true, FDetailsViewArgs::ObjectsUseNameArea);
	args.ViewIdentifier = "WeaponAssetEditorDetailsView"; // Detail View�� ��� �� �̸�
	DetailsView = prop.CreateDetailView(args); // ���� ���õǾ��ִ� Object�� �⺻���� Detail View�� ���̵��� ��

	/* Editor ������ �г� �ٹ̱� */
	FOnGetDetailCustomizationInstance detailsView = FOnGetDetailCustomizationInstance::CreateStatic(&SWeaponDetailsView::MakeInstance);
	DetailsView->SetGenericLayoutDetailsDelegate(detailsView);

	/* PropertyType Customization */
	prop.RegisterCustomPropertyTypeLayout("EquipmentData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SEquipmentData::MakeInstance));
	prop.RegisterCustomPropertyTypeLayout("DoActionData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SDoActionData::MakeInstance));
	prop.RegisterCustomPropertyTypeLayout("HitData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SHitData::MakeInstance));

	/* Layout ���� */
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
				->AddTab(LeftAreaTabId, ETabState::OpenedTab)
				->SetHideTabWell(true) // ���� ������ ���ϵ��� ����
			)
			->Split
			(
				FTabManager::NewStack() 
				->SetSizeCoefficient(0.725f) // �������� ������ 0.725
				->AddTab(DetailTabId, ETabState::OpenedTab)
				->SetHideTabWell(true) // ���� ������ ���ϵ��� ����
			)
		)
	);

	/* Detail View�� �⺻������ � WeaponAsset�� �������� �ʱ�ȭ�ϱ� */
	UCWeaponAsset* asset = nullptr;

	// DataAsset ������ ���� Ŭ���ؼ� ���� InAssetName = ���ϸ�
	// ToolBar�� ��ư���� WeaponAssetEditor�� ���� InAssetName = �������
	if (InAssetName.Len() > 0)
	{
		FWeaponRowDataPtr ptr = LeftArea->GetRowDataPtrByName(InAssetName);

		// �̹� ������â���� Hammer�� ������µ�, DA_Hammer�� �� �����ؼ� ���� �ٸ� �۾��� �� �ʿ䰡 ����
		// �̷� �۾��� ������ ����
		if (LeftArea->SelectedRowDataPtrName() == InAssetName)
			return;

		// InAssetName�� ���������� ���õ� ������ �׸��� ���� ��� -> ���� �ּ��� ������ �ȳѾ�°�
		if (ptr.IsValid() == false) 
			asset = LeftArea->GetFirstDataPtr()->Asset;
		// �Ϲ����� ��쿣 ������ ������ �׸��� �״�� ����
		else
			asset = ptr->Asset;
	}
	else // InAssetName.Len �� 0���� ������ ToolBar�� ��ư���� Editor�� �������Ƿ� ù��° ListView �������׸��� Default�� ���
		asset = LeftArea->GetFirstDataPtr()->Asset;

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, false, true, asset);
	// SelectDataPtr���� Detail���� �������ֹǷ� SetObject�� �ƴ� SelectDataPtr �Լ� ���
	LeftArea->SelectDataPtr(asset);
}

/* â�� �ݴ� ��û�� ���� ��� */
bool FWeaponAssetEditor::OnRequestClose()
{
	if (!!DetailsView)
	{
		if (!!GEditor && !!GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this); // �������� �����Ͱ� �����ٰ� �˷���

		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			prop.UnregisterCustomPropertyTypeLayout("EquipmentData");
			prop.UnregisterCustomPropertyTypeLayout("DoActionData");
			prop.UnregisterCustomPropertyTypeLayout("HitData");
		}
	}

	if (LeftArea.IsValid())
		LeftArea.Reset();

	if (DetailsView.IsValid())
		DetailsView.Reset();

	return true;
}

void FWeaponAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab tab;
	tab.BindSP(this, &FWeaponAssetEditor::Spawn_LeftAreaTab); // SP : Slate Pointer
	TabManager->RegisterTabSpawner(LeftAreaTabId, tab);

	FOnSpawnTab tab2;
	tab2.BindSP(this, &FWeaponAssetEditor::Spawn_DetailsViewTab);
	TabManager->RegisterTabSpawner(DetailTabId, tab2);
}

// SDockTab : Tab �ϳ��� �� ����
TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_LeftAreaTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
	[
		LeftArea.ToSharedRef()
	];
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_DetailsViewTab(const FSpawnTabArgs & InArgs)
{
	return SNew(SDockTab)
	[
		DetailsView.ToSharedRef()
	];
}

void FWeaponAssetEditor::OnListViewSelectedItem(FWeaponRowDataPtr InPtr)
{
	if (InPtr == nullptr) return;

	if (!!GetEditingObject()) // �������� Object�� �����Ѵٸ�
		RemoveEditingObject(GetEditingObject());

	AddEditingObject(InPtr->Asset);
	DetailsView->SetObject(InPtr->Asset);
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



