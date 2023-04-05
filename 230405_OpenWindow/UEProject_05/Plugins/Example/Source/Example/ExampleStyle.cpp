#include "ExampleStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

const FName FExampleStyle::StyleSetName = "ExampleStyle";

TSharedPtr<FExampleStyle> FExampleStyle::Instance = nullptr;

TSharedRef<FExampleStyle> FExampleStyle::Get()
{
	if (Instance == nullptr)
		Instance = MakeShareable(new FExampleStyle());

	return Instance.ToSharedRef();
}

void FExampleStyle::ShutDown()
{
	if (Instance.IsValid())
		Instance.Reset();
}

FExampleStyle::FExampleStyle()
{
	StyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));

	FString path = IPluginManager::Get().FindPlugin("Example")->GetBaseDir();
	path = path / "Resources"; // '/' 는 문자열을 연결
	StyleSet->SetContentRoot(path);

	//GLog->Log(ELogVerbosity::Warning, path);

	RegisterIcon("ToolBar_Icon", path / "T_Radial_Local_Space.png", FVector2D(36, 36), ToolBar_Icon);
	RegisterIcon("ToolBar_Icon2", path / "icon_MatEd_Home_40x.png", FVector2D(36, 36), ToolBar_Icon2);

	// Style Set 자체를 Register에 저장
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

FExampleStyle::~FExampleStyle()
{
	if (StyleSet.IsValid() == false) return;

	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSetName);
	StyleSet.Reset();
}

void FExampleStyle::RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize,
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

