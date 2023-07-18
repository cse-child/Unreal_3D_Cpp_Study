#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"


UCLASS()
class UEPROJECT_06_API UCWeaponData : public UObject
{
	GENERATED_BODY()

public:
	friend class UCWeaponAsset;		// CWeaponAsset을 통해서만 CWeaponData에 접근할 수 있도록 설정

public:
	FORCEINLINE class ACAttachment* GetAttachment() const { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() const { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() const { return DoAction; }
	FORCEINLINE class UCSubAction* GetSubAction() const { return SubAction; }

private:
	UPROPERTY()
		class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCDoAction* DoAction;

	UPROPERTY()
		class UCSubAction* SubAction;
};


