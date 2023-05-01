#include "WeaponContextMenu.h"
#include "WeaponAssetEditor.h"
#include "Weapons/CWeaponAsset.h"

FWeaponContextMenu::FWeaponContextMenu(EAssetTypeCategories::Type InType)
{
	Category = InType;
}

void FWeaponContextMenu::OpenAssetEditor(const TArray<UObject*>& InObjects,TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	// 창을 열 때
	//FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);

	if (InObjects.Num() < 1)
		return;

	FWeaponAssetEditor::OpenWindow(InObjects[0]->GetName());
}

FText FWeaponContextMenu::GetName() const
{
	return FText::FromString("DataAsset");
}

// SuppertedClass를 사용하려면 반드시 Factory가 있어야 한다.
UClass* FWeaponContextMenu::GetSupportedClass() const
{
	return UCWeaponAsset::StaticClass();
}

// Type마다의 고유한 색상
FColor FWeaponContextMenu::GetTypeColor() const
{
	return FColor::Blue; 
}

uint32 FWeaponContextMenu::GetCategories()
{
	return Category;
}
