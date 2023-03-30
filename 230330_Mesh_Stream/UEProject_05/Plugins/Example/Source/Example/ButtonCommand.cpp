#include "ButtonCommand.h"

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
	UI_COMMAND(ID, "LoadMesh", "", EUserInterfaceActionType::Button, FInputChord());

#undef LOCTEXT_NAMESPACE
	Command->MapAction(ID, FExecuteAction::CreateRaw(this, &FButtonCommand::OnClicked_ID), FCanExecuteAction());
}

void FButtonCommand::OnClicked_ID()
{
	GLog->Log("Test");
}            
