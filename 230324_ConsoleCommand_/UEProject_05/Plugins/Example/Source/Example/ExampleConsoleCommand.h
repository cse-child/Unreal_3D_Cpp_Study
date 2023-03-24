#pragma once

#include "CoreMinimal.h"

class EXAMPLE_API FExampleConsoleCommand
{
public:
	FExampleConsoleCommand();
	~FExampleConsoleCommand();

private:
	void OpenWindow(FString InString);

private:
	struct IConsoleCommand* OpenWindowCommand;
	struct IConsoleCommand* OpenWindowArgsCommand;
};


