#include "ExampleModule.h"

#include "ExampleDebuggerCategory.h"
#include "GameplayDebugger.h"
#include "ExampleConsoleCommand.h"

#include "ButtonCommand.h"
#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "FExampleModule"

IMPLEMENT_MODULE(FExampleModule, Example)

void FExampleModule::StartupModule()
{
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
		FButtonCommand::Get().ID,
		NAME_None,
		FText::FromString("Load Mesh"),
		FText::FromString("Load Mesh Data")
	);
}

#undef LOCTEXT_NAMESPACE
	
