#include "ExampleModule.h"

#include "ExampleStyle.h"

#include "ExampleDebuggerCategory.h"
#include "GameplayDebugger.h"
#include "ExampleConsoleCommand.h"

#include "ButtonCommand.h"
#include "LevelEditor.h"

#include "CStaticMesh.h"
#include "StaticMesh_Detail.h"

#define LOCTEXT_NAMESPACE "FExampleModule"

IMPLEMENT_MODULE(FExampleModule, Example)

void FExampleModule::StartupModule()
{
	FExampleStyle::Get();

	// Gameplay Debugger
	{
		IGameplayDebugger& debugger = IGameplayDebugger::Get();
		IGameplayDebugger::FOnGetCategory category = IGameplayDebugger::FOnGetCategory::CreateStatic(&FExampleDebuggerCategory::MakeInstance);
		debugger.Get().RegisterCategory("Example", category, EGameplayDebuggerCategoryState::EnabledInGameAndSimulate);
		debugger.NotifyCategoriesChanged();
	}

	// Console Command
	{
		ConsoleCommand = MakeShareable(new FExampleConsoleCommand());
	}

	 //ToolBar
	{
		FButtonCommand::Register();
	
		Extender = MakeShareable(new FExtender());
	
		FToolBarExtensionDelegate toolBar = FToolBarExtensionDelegate::CreateRaw(this, &FExampleModule::AddToolBar);
		Extender->AddToolBarExtension("Compile", EExtensionHook::Before, FButtonCommand::Get().Command, toolBar);
	
		FLevelEditorModule& levelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		levelEditor.GetToolBarExtensibilityManager()->AddExtender(Extender);
	}

	// Detail Panel
	{
		FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		prop.RegisterCustomClassLayout
		(
			ACStaticMesh::StaticClass()->GetFName(),
			FOnGetDetailCustomizationInstance::CreateStatic(&FStaticMesh_Detail::MakeInstance)
		);
	}
}

void FExampleModule::ShutdownModule()
{
	if (IGameplayDebugger::IsAvailable())
		IGameplayDebugger::Get().UnregisterCategory("Example");

	if (ConsoleCommand.IsValid())
		ConsoleCommand.Reset();
}

void FExampleModule::AddToolBar(FToolBarBuilder& InBuilder)
{
	InBuilder.AddSeparator();
	InBuilder.AddToolBarButton
	(
		FButtonCommand::Get().LoadMesh,
		NAME_None,
		FText::FromString("Load Mesh"),
		FText::FromString("Load Mesh Data"),
		FExampleStyle::Get()->ToolBar_Icon
	);

	InBuilder.AddToolBarButton
	(
		FButtonCommand::Get().OpenWindow,
		NAME_None,
		FText::FromString("Open Window"),
		FText::FromString("Open mesh view window"),
		FExampleStyle::Get()->ToolBar_Icon2
	);
}

#undef LOCTEXT_NAMESPACE
	
