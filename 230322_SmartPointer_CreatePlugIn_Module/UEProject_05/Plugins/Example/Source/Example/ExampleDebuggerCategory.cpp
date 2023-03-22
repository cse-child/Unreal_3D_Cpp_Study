#include "ExampleDebuggerCategory.h"

FExampleDebuggerCategory::FExampleDebuggerCategory()
{
	//GLog->Log(ELogVerbosity::Warning, "FExampleDebuggerCategory Start");
}

FExampleDebuggerCategory::~FExampleDebuggerCategory()
{
}

TSharedRef<FGameplayDebuggerCategory> FExampleDebuggerCategory::MakeInstance()
{
	return MakeShareable(new FExampleDebuggerCategory());
}

