#pragma once

#include "CoreMinimal.h"

class EXAMPLE_API FExampleConsoleCommand
{
public:
	FExampleConsoleCommand();
	~FExampleConsoleCommand();

private:
	void DisplayWindow(const TArray<FString>& InArgs);

private:
	IConsoleCommand* DisplayTestCommand;
	IConsoleCommand* DisplayWindowCommand;
};


