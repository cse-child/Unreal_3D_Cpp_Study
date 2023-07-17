#include "WeaponStyle.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

const FName FWeaponStyle::StyleSetName = "WeaponStyle";

TSharedPtr<FWeaponStyle> FWeaponStyle::Instance = nullptr;

TSharedRef<FWeaponStyle> FWeaponStyle::Get()
{
	if (Instance == nullptr)
		Instance = MakeShareable(new FWeaponStyle());

	return Instance.ToSharedRef();
}

void FWeaponStyle::ShutDown()
{
	if (Instance.IsValid())
		Instance.Reset();
}

FWeaponStyle::FWeaponStyle()
{
	StyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));

	FString path = "";

	path = FPaths::ProjectPluginsDir() / "Weapon" / "Resources";
	RegisterIcon("ToolBar_Icon", path / "weapon_thumnail_icon.png", FVector2D(40, 40), ToolBar_Icon);

	// Style Set 자체를 Register에 저장
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());

	path = FPaths::EngineContentDir() / "Editor" / "Slate" / "Common/Selection.png";
	Array_Image = MakeShareable(new FSlateImageBrush(path, FVector2D(8, 8), FLinearColor(1, 1, 1, 0.1f)));
}

FWeaponStyle::~FWeaponStyle()
{
	if (Array_Image.IsValid())
		Array_Image.Reset();

	if (StyleSet.IsValid() == false) return;

	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSetName);
	StyleSet.Reset();
}

void FWeaponStyle::RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize,
	FSlateIcon& OutSlateIcon)
{
	
}

