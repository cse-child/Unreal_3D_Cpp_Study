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
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE class UCSubAction* GetSubAction() { return SubAction; }

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

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCSubAction> SubActionClass;

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

	UPROPERTY()
		class UCSubAction* SubAction;


#if WITH_EDITOR
	void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;

#endif // WITH_EDITOR
};



