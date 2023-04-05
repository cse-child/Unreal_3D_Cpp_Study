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
	archive.Seek(0); // Seek : 파일을 읽는 포인터

	FStaticMesh_DetailData data;
	reader << data; // 언리얼은 stream 방향을 << 으로 통일
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

	// 카메라가 바라보는 방향에 충돌된 지점에 Mesh를 배치
	FLevelEditorViewportClient* client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient(); // GetClient(): Viewport 카메라

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

	/* 객체 생성 위치,방향 설정 */
	FTransform transform;
	FVector direction = (hitResult.TraceEnd - hitResult.TraceStart);
	direction.Normalize();

	FVector location = hitResult.TraceStart + direction * (hitResult.Distance - data.Radius);
	transform.SetLocation(location);

	FRotator rotation = FRotator(0, direction.Rotation().Yaw, 0);
	transform.SetRotation(FQuat(rotation));

	/* 객체 생성 */
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

	actor->FinishSpawning(transform); // 배치 위치가 확정됨
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

	/* Window 객체 생성 */
	FExampleMeshWindow::OpenWindow(NewObject<AStaticMeshActor>()); // NewObject : Runtime 에서의 동적할당

}            

