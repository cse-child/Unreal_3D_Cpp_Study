#pragma once

#include "CoreMinimal.h"

class EXAMPLE_API FExampleConsoleCommand
{
public:
	FExampleConsoleCommand();
	~FExampleConsoleCommand();

private:
	void OpenWindow(FString InString);
	void MoveToPlayer(const TArray<FString>& InArgs, UWorld* InWorld);

private:
	struct IConsoleCommand* OpenWindowCommand;
	struct IConsoleCommand* OpenWindowArgsCommand;
	struct IConsoleCommand* OpenWindowWorldArgsCommand;
};


