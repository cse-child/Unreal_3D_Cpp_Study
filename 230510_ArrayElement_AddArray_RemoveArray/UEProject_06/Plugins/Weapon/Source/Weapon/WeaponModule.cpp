#include "WeaponModule.h"
#include "WeaponStyle.h"
#include "WeaponCommand.h"
#include "WeaponContextMenu.h"
#include "SEquipmentData.h"
#include "SDoActionData.h"
#include "SHitData.h"

#include "IAssetTools.h"
#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "FWeaponModule"

IMPLEMENT_MODULE(FWeaponModule, Weapon)

void FWeaponModule::StartupModule()
{
	FWeaponStyle::Get();

	Command = MakeShareable(new FWeaponCommand());
	Command->Startup();

	IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
	EAssetTypeCategories::Type type = assetTools.RegisterAdvancedAssetCategory("WeaponAsset", FText::FromString("Weapon"));

	ContextMenu = MakeShareable(new FWeaponContextMenu(type));
	assetTools.RegisterAssetTypeActions(ContextMenu.ToSharedRef());

	FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	prop.RegisterCustomPropertyTypeLayout("EquipmentData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SEquipmentData::MakeInstance));
	prop.RegisterCustomPropertyTypeLayout("DoActionData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SDoActionData::MakeInstance));
	prop.RegisterCustomPropertyTypeLayout("HitData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SHitData::MakeInstance));
}

void FWeaponModule::ShutdownModule()
{
	if (Command.IsValid())
		Command.Reset();

	if (ContextMenu.IsValid())
		ContextMenu.Reset();

	FWeaponStyle::ShutDown();

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		prop.UnregisterCustomPropertyTypeLayout("EquipmentData");
		prop.UnregisterCustomPropertyTypeLayout("DoActionData");
		prop.UnregisterCustomPropertyTypeLayout("HitData");
	}
}

#undef LOCTEXT_NAMESPACE