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

	FString path = FPaths::ProjectPluginsDir() / "Weapon" / "Resources";
	StyleSet->SetContentRoot(path);

	RegisterIcon("ToolBar_Icon", path / "weapon_thumnail_icon.png", FVector2D(36, 36), ToolBar_Icon);

	// Style Set 자체를 Register에 저장
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

FWeaponStyle::~FWeaponStyle()
{
	if (StyleSet.IsValid() == false) return;

	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSetName);
	StyleSet.Reset();
}

void FWeaponStyle::RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize,
	FSlateIcon& OutSlateIcon)
{
	// InName : Style 각각의 이름
	// brush : 뭔가를 표현하기 위한 객체 (image brush, font brush..)
	FSlateImageBrush* brush = new FSlateImageBrush(InPath, InIconSize);

	FString name = StyleSetName.ToString() + "." + InName; // StyleSetName 앞에 . 을 붙이는 것이 규칙
	StyleSet->Set(FName(name), brush); // 사용할 Style 등록

	// 실제로 사용할 Icon 생성
	OutSlateIcon = FSlateIcon(FName(StyleSetName), FName(name));
}

