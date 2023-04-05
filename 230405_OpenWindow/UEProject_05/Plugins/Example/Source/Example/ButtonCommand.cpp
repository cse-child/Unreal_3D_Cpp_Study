#include "ButtonCommand.h"
#include "StaticMesh_Detail.h"
#include "CStaticMesh_Copied.h"
#include "ExampleMeshWindow.h"
#include "DesktopPlatformModule.h"
#include "LevelEditorViewport.h"
#include "Engine/Selection.h"
#include "Interfaces/IMainFrameModule.h"
#include "Misc/MessageDialog.h"
#include "Serialization/BufferArchive.h"
#include "Misc/FileHelper.h"
#include "GameFramework/PlayerController.h"
#include "Engine/StaticMeshActor.h"

FButtonCommand::FButtonCommand()
	:TCommands("Toolbar_Buttons", FText::FromString(""), NAME_None, FEditorStyle::GetStyleSetName())
{
	Command = MakeShareable(new FUICommandList());
}

FButtonCommand::~FButtonCommand()
{
	if (Command.IsValid())
		Command.Reset();
}

void FButtonCommand::RegisterCommands()
{
#define LOCTEXT_NAMESPACE ""
	UI_COMMAND(LoadMesh, "LoadMesh", "", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenWindow, "OpenWindow", "", EUserInterfaceActionType::Button, FInputChord());

#undef LOCTEXT_NAMESPACE
	Command->MapAction(LoadMesh, FExecuteAction::CreateRaw(this, &FButtonCommand::OnClicked_LoadMesh), FCanExecuteAction());
	Command->MapAction(OpenWindow, FExecuteAction::CreateRaw(this, &FButtonCommand::OnClicked_OpenWindow), FCanExecuteAction());
}

void FButtonCommand::OnClicked_LoadMesh()
{
	//if (FApp::IsGame())
	//{
	//	FMessageDialog dialog;
	//	dialog.Debugf(FText::FromString("In Game mode not working"));
	//
	//	return;
	//}

	//bool b = FApp::IsGame();
	//GLog->Log(b ? "T" : "F");

	FString path;
	FPaths::GetPath(path);

	IMainFrameModule& mainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	void* handle = mainFrame.GetParentWindow()->GetNativeWindow()->GetOSWindowHandle();

	IDesktopPlatform* platform = FDesktopPlatformModule::Get();

	TArray<FString> fileNames;
	platform->OpenFileDialog(handle, "Open Mesh File", path, "", "Mesh Binary File(*.bin)|*.bin", EFileDialogFlags::None, fileNames);
	if (fileNames.Num() < 1) return;

	///////////////////////////////////////////////////////////////////////

	FBufferArchive archive;
	FFileHelper::LoadFileToArray(archive, *fileNames[0]);

	// FMemoryReader : Output Stream
	FMemoryReader reader = FMemoryReader(archive, true);
	archive.Seek(0); // Seek : ������ �д� ������

	FStaticMesh_DetailData data;
	reader << data; // �𸮾��� stream ������ << ���� ����
	reader.FlushCache();
	reader.Close();

	GLog->Logf(L"Vertex Count : %d", data.Positions.Num());
	GLog->Logf(L"Triangle Count : %d", data.Indices.Num() / 3);
	GLog->Logf(L"Extent : %s", *data.Extent.ToString());
	GLog->Logf(L"Radius : %f", data.Radius);

	FString text;
	for (int32 i = 0; i < data.Positions.Num(); i++)
	{
		text.Append(data.Positions[i].ToString() + ", ");
		text.Append(data.Normals[i].ToString() + ", ");
		text.Append(data.Uvs[i].ToString() + ", ");
		text.Append(data.Colors[i].ToString() + "\r\n");
	}

	FString textFileName = FPaths::GetBaseFilename(fileNames[0], false);
	FString textSaveName = textFileName + "_Copied.csv";

	FFileHelper::SaveStringToFile(text, *textSaveName);

	///////////////////////////////////////////////////////////////////////////

	// ī�޶� �ٶ󺸴� ���⿡ �浹�� ������ Mesh�� ��ġ
	FLevelEditorViewportClient* client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient(); // GetClient(): Viewport ī�޶�

	FVector start = client->GetViewLocation();
	FVector end = start + client->GetViewRotation().RotateVector(FVector(10000, 0, 0));

	FHitResult hitResult;
	UWorld* world = GEditor->GetEditorWorldContext().World();

	world->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility);
	if (hitResult.bBlockingHit == false)
	{
		FMessageDialog dialog;
		dialog.Debugf(FText::FromString("Can't be placed in this location"));

		return;
	}

	///////////////////////////////////////////////////////////////////////////

	/* ��ü ���� ��ġ,���� ���� */
	FTransform transform;
	FVector direction = (hitResult.TraceEnd - hitResult.TraceStart);
	direction.Normalize();

	FVector location = hitResult.TraceStart + direction * (hitResult.Distance - data.Radius);
	transform.SetLocation(location);

	FRotator rotation = FRotator(0, direction.Rotation().Yaw, 0);
	transform.SetRotation(FQuat(rotation));

	/* ��ü ���� */
	ACStaticMesh_Copied* actor = world->SpawnActorDeferred<ACStaticMesh_Copied>
	(
		ACStaticMesh_Copied::StaticClass(),
		transform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);

	actor->SetPositions(data.Positions);
	actor->SetIndices(data.Indices);
	actor->SetNormals(data.Normals);
	actor->SetUvs(data.Uvs);
	actor->SetColors(data.Colors);

	actor->FinishSpawning(transform); // ��ġ ��ġ�� Ȯ����
}

void FButtonCommand::OnClicked_OpenWindow()
{
	USelection* selection =  GEditor->GetSelectedActors();
	UObject* obj = selection->GetSelectedObject(0);

	AStaticMeshActor* actor = Cast<AStaticMeshActor>(obj);
	if(!!actor)
	{
		FExampleMeshWindow::OpenWindow(actor);

		return;
	}

	FMessageDialog::Debugf(FText::FromString("Must be selected AStaticMeshActor type"));

	/* Window ��ü ���� */
	FExampleMeshWindow::OpenWindow(NewObject<AStaticMeshActor>()); // NewObject : Runtime ������ �����Ҵ�

}            

