#include "ButtonCommand.h"
#include "StaticMesh_Detail.h"
#include "DesktopPlatformModule.h"
#include "Interfaces/IMainFrameModule.h"
#include "Misc/MessageDialog.h"
#include "Serialization/BufferArchive.h"

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

#undef LOCTEXT_NAMESPACE
	Command->MapAction(LoadMesh, FExecuteAction::CreateRaw(this, &FButtonCommand::OnClicked_LoadMesh), FCanExecuteAction());
}

void FButtonCommand::OnClicked_LoadMesh()
{

	if (FApp::IsGame())
	{
		FMessageDialog dialog;
		dialog.Debugf(FText::FromString("In Game mode not working"));

		return;
	}

	FString path;
	FPaths::GetPath(path);

	IMainFrameModule& mainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	void* handle = mainFrame.GetParentWindow()->GetNativeWindow()->GetOSWindowHandle();

	IDesktopPlatform* platform = FDesktopPlatformModule::Get();

	TArray<FString> fileNames;
	platform->SaveFileDialog(handle, "Open Mesh File", path, "", "Mesh Binary File(*.bin)|*.bin", EFileDialogFlags::None, fileNames);
	if (fileNames.Num() < 1) return;

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


}            
