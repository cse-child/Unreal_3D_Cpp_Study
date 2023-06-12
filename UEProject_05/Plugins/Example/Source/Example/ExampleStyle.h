#pragma once

#include "CoreMinimal.h"

class EXAMPLE_API FExampleStyle
{
public:
	static TSharedRef<FExampleStyle> Get();
	static void ShutDown();

private:
	static TSharedPtr<FExampleStyle> Instance;
	
public:
	FExampleStyle();
	~FExampleStyle();

private:
	void RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize, FSlateIcon& OutSlateIcon);

private:
	static const FName StyleSetName;
	TSharedPtr<class FSlateStyleSet> StyleSet;

public:
	FSlateIcon ToolBar_Icon;
	FSlateIcon ToolBar_Icon2;
};


