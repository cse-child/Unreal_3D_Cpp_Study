#include "ExampleConsoleCommand.h"
#include "HAL/IConsoleManager.h" // HAL(Hardware Abstraction Layer): �ϵ������ �߻�ȭ, �ϵ��� ������� �۵���

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
