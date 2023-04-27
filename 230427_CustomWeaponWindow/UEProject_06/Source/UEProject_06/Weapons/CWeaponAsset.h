#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWeaponStructures.h"
#include "CWeaponAsset.generated.h"


UCLASS()
class UEPROJECT_06_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE class ACAttachment* GetAttackment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
		TArray<FHitData> HitDatas;

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);

private:
	UPROPERTY() // 직렬화를 하면 null이 되는 순간이 와도 가비지컬렉터가 삭제하지 않음
		class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCDoAction* DoAction;
};

