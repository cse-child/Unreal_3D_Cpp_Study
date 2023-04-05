#include "ExampleMeshWindow.h"
#include "Engine/StaticMeshActor.h"

const FName FExampleMeshWindow::EditorName = "MeshWindow";
const FName FExampleMeshWindow::ViewportTabId = "Viewport";
const FName FExampleMeshWindow::DetailsTabId = "Details";
const FName FExampleMeshWindow::PreviewTabId = "Preview";

TSharedPtr<FExampleMeshWindow> FExampleMeshWindow::Instance = nullptr;

void FExampleMeshWindow::OpenWindow(UObject* InAsset)
{
	if(Instance.IsValid()) // Instance�� �̹� �ִ� == â�� �����ִ�
	{
		Instance->CloseWindow();

		Instance.Reset();
		Instance = nullptr;
	}

	Instance = MakeShareable(new FExampleMeshWindow());
	Instance->Open(InAsset);
}

void FExampleMeshWindow::Shutdown()
{
	if (Instance.IsValid())
		Instance->CloseWindow();
}

void FExampleMeshWindow::Open(UObject* InAsset)
{
	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("MeshWindow_Layout")
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->AddTab(GetToolbarTabId(), ETabState::OpenedTab) // â�� �������·� ���ϰ��ΰ�?
		)
		->Split
		(
			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
			->SetSizeCoefficient(0.75f)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(ViewportTabId, ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
				->SetSizeCoefficient(0.25f)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(PreviewTabId, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(DetailsTabId, ETabState::OpenedTab)
				)
			)
		)
	);

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, true, true, InAsset);
}

FName FExampleMeshWindow::GetToolkitFName() const
{
	return EditorName;
}

FText FExampleMeshWindow::GetBaseToolkitName() const
{
	return FText::FromName(EditorName);
}

// Tab �� ��Ÿ�� �̸�
FString FExampleMeshWindow::GetWorldCentricTabPrefix() const
{
	return EditorName.ToString();
}

// ������ ���� ����
FLinearColor FExampleMeshWindow::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0, 0, 1);
}
