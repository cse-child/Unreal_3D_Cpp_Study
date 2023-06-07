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
		// 이미 에디터 창이 열려있는 상태에서 다른 DA 클래스를 누른것
		// -> 누른 항목으로 열린 에디터 창을 변경시킨다.
		// 이부분 코드가 없으면 다른 DA클래스 열 때 창이 껐다켜졌다 함
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
	/* ListView 객체 생성 */
	LeftArea = SNew(SWeaponLeftArea).OnListViewSelectedItem(this, &FWeaponAssetEditor::OnListViewSelectedItem);

	/* Detail Tab 객체 생성 */
	// FPropertyEditorModule : 엔진에 존재하는 모든 Detail View를 관할하는 모듈
	FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	{
	/*
	FDetailsViewArgs 매개변수
	
	const bool InUpdateFromSelection			: 선택할때마다 Update
	const bool InLockable						: 편집할 수 없도록 잠그기
	const bool InAllowSearch					: 돋보기(찾기) 기능의 허용여부
	const ENameAreaSettings InNameAreaSettings
	-> ActorsUserNameArea						: Actor의 이름을 사용
	-> ComponentsAndActorsUseNameArea			: Component와 Actor의 이름 사용
	-> ObjectsUseNameArea						: Object의 이름 사용
	const bool InHideSelectionTip				: 선택한 오브젝트가 없을 때 Tip을 표시
	FNotifyHook* InNotifyHook					: 속성이 변경되면 호출하는 Hook
	const bool InSearchInitialKeyFocus			: 검색창에 초기 키보드 포커스를 두기
	FName InViewIdentifier						: 보기용 식별자
	*/
	}
	FDetailsViewArgs args(false, false, true, FDetailsViewArgs::ObjectsUseNameArea);
	args.ViewIdentifier = "WeaponAssetEditorDetailsView"; // Detail View를 띄울 때 이름
	DetailsView = prop.CreateDetailView(args); // 현재 선택되어있는 Object의 기본적인 Detail View를 보이도록 함

	/* Editor 디테일 패널 꾸미기 */
	FOnGetDetailCustomizationInstance detailsView = FOnGetDetailCustomizationInstance::CreateStatic(&SWeaponDetailsView::MakeInstance);
	DetailsView->SetGenericLayoutDetailsDelegate(detailsView);

	/* PropertyType Customization */
	prop.RegisterCustomPropertyTypeLayout("EquipmentData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SEquipmentData::MakeInstance));
	prop.RegisterCustomPropertyTypeLayout("DoActionData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SDoActionData::MakeInstance));
	prop.RegisterCustomPropertyTypeLayout("HitData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SHitData::MakeInstance));

	/* Layout 생성 */
	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("WeaponAssetEditor_Layout")
	->AddArea
	(
		/* ToolBar 영역 */
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
		)
		/* Tab 영역 */
		->Split
		(
			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal) // 수평 공간 분할
			->Split
			(
				FTabManager::NewStack() // NewStack은 하나의 공간
				->SetSizeCoefficient(0.175f) // 왼쪽의 비율이 0.175
				->AddTab(LeftAreaTabId, ETabState::OpenedTab)
				->SetHideTabWell(true) // 탭을 숨기지 못하도록 설정
			)
			->Split
			(
				FTabManager::NewStack() 
				->SetSizeCoefficient(0.725f) // 오른쪽의 비율이 0.725
				->AddTab(DetailTabId, ETabState::OpenedTab)
				->SetHideTabWell(true) // 탭을 숨기지 못하도록 설정
			)
		)
	);

	/* Detail View에 기본적으로 어떤 WeaponAsset을 띄울것인지 초기화하기 */
	UCWeaponAsset* asset = nullptr;

	// DataAsset 파일을 직접 클릭해서 열면 InAssetName = 파일명
	// ToolBar의 버튼으로 WeaponAssetEditor를 열면 InAssetName = 비어있음
	if (InAssetName.Len() > 0)
	{
		FWeaponRowDataPtr ptr = LeftArea->GetRowDataPtrByName(InAssetName);

		// 이미 에디터창으로 Hammer를 열어놨는데, DA_Hammer를 또 선택해서 열면 다른 작업을 할 필요가 없음
		// 이런 작업을 막도록 설정
		if (LeftArea->SelectedRowDataPtrName() == InAssetName)
			return;

		// InAssetName은 존재하지만 선택된 데이터 항목이 없는 경우 -> 뭔가 애셋이 꼬여서 안넘어온것
		if (ptr.IsValid() == false) 
			asset = LeftArea->GetFirstDataPtr()->Asset;
		// 일반적인 경우엔 선택한 데이터 항목을 그대로 적용
		else
			asset = ptr->Asset;
	}
	else // InAssetName.Len 이 0보다 작으면 ToolBar의 버튼으로 Editor을 열었으므로 첫번째 ListView 데이터항목을 Default로 띄움
		asset = LeftArea->GetFirstDataPtr()->Asset;

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, false, true, asset);
	// SelectDataPtr에서 Detail까지 셋팅해주므로 SetObject가 아닌 SelectDataPtr 함수 사용
	LeftArea->SelectDataPtr(asset);
}

/* 창을 닫는 요청이 들어온 경우 */
bool FWeaponAssetEditor::OnRequestClose()
{
	if (!!DetailsView)
	{
		if (!!GEditor && !!GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this); // 편집중인 에디터가 닫혔다고 알려줌

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

// SDockTab : Tab 하나에 들어갈 공간
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

	if (!!GetEditingObject()) // 편집중인 Object가 존재한다면
		RemoveEditingObject(GetEditingObject());

	AddEditingObject(InPtr->Asset);
	DetailsView->SetObject(InPtr->Asset);
}

FName FWeaponAssetEditor::GetToolkitFName() const
{
	return EditorName; // 편집할 창 이름
}

FText FWeaponAssetEditor::GetBaseToolkitName() const
{
	return FText::FromName(EditorName); // 언리얼에서 식별할 이름
}

FString FWeaponAssetEditor::GetWorldCentricTabPrefix() const
{
	return EditorName.ToString(); // 화면에 나타날 이름
}

FLinearColor FWeaponAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0, 0, 1); // 창에서 편집할 에셋의 색상
}



