#include "WeaponContextMenu.h"
#include "Weapons/CWeaponAsset.h"

FWeaponContextMenu::FWeaponContextMenu(EAssetTypeCategories::Type InType)
{
	Category = InType;
}

void FWeaponContextMenu::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	//FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
	FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);

}

FText FWeaponContextMenu::GetName() const
{
	return FText::FromString("DataAsset");
}

// SuppertedClass�� ����Ϸ��� �ݵ�� Factory�� �־�� �Ѵ�.
UClass* FWeaponContextMenu::GetSupportedClass() const
{
	return UCWeaponAsset::StaticClass();
}

// Type������ ������ ����
FColor FWeaponContextMenu::GetTypeColor() const
{
	return FColor::Blue; 
}

uint32 FWeaponContextMenu::GetCategories()
{
	return Category;
}
