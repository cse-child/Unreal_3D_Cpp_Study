#include "ExampleConsoleCommand.h"
#include "HAL/IConsoleManager.h" // HAL(Hardware Abstraction Layer): 하드웨어의 추상화, 하드웨어에 상관없이 작동함

FExampleConsoleCommand::FExampleConsoleCommand()
{
	TArray<FString> args;
	args.Add("Test Command Window");

	FConsoleCommandWithArgsDelegate command = FConsoleCommandWithArgsDelegate::CreateRaw(this, &FExampleConsoleCommand::DisplayWindow);


	DisplayTestCommand = IConsoleManager::Get().RegisterConsoleCommand
	(
		L"Display Example Command Window", L"None Help", command
	);
}

FExampleConsoleCommand::~FExampleConsoleCommand()
{

}

void FExampleConsoleCommand::DisplayWindow(const TArray<FString>& InArgs)
{
}
