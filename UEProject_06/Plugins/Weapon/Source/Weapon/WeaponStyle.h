#pragma once

#include "CoreMinimal.h"

class WEAPON_API FWeaponStyle
{
public:
	static TSharedRef<FWeaponStyle> Get();
	static void ShutDown();

private:
	static TSharedPtr<FWeaponStyle> Instance;
	
public:
	FWeaponStyle();
	~FWeaponStyle();

private:
	void RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize, FSlateIcon& OutSlateIcon);

private:
	static const FName StyleSetName;
	TSharedPtr<class FSlateStyleSet> StyleSet;

public:
	FSlateIcon ToolBar_Icon;
	TSharedPtr<struct FSlateImageBrush> Array_Image;

public:
	FVector2D DesiredWidth = FVector2D(100, 1000);
};


