#include "SWeaponDetailsView.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "Weapons/CWeaponAsset.h"

TSharedRef<IDetailCustomization> SWeaponDetailsView::MakeInstance()
{
	GLog->Log("MakeInstance");
	return MakeShareable(new SWeaponDetailsView());
}

void SWeaponDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type = UCWeaponAsset::StaticClass();

	// Attachment
	{
		// ī�װ��� ������ Return, ������ �߰���
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Attachment", FText::FromName("Attachment"));
		category.AddProperty("AttachmentClass", type); // ����ȭ�� ��ü�� �̸��� �ִٸ� �����ͼ� �־���
	}

	// Equipment
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Equipment", FText::FromName("Equipment"));
		category.AddProperty("EquipmentClass", type);

		category.AddProperty("EquipmentData", type);
		
	}
}