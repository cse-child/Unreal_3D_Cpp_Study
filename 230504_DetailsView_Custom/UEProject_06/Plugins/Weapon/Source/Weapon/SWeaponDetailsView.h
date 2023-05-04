#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"


class WEAPON_API SWeaponDetailsView : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance(); // 창이 열릴때마다
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override; // 창이 열리고 난 뒤 세부내용
};
