#include "ExampleModule.h"

#include "ExampleDebuggerCategory.h"
#include "GameplayDebugger.h"

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
}

void FExampleModule::ShutdownModule()
{
	if (IGameplayDebugger::IsAvailable())
		IGameplayDebugger::Get().UnregisterCategory("Example");
}

#undef LOCTEXT_NAMESPACE
	
