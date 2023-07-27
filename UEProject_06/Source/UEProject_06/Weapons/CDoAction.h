#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.h"
#include "CDoAction.generated.h"


UCLASS(Abstract, NotBlueprintable)
class UEPROJECT_06_API UCDoAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }

public:
	UCDoAction();

	virtual void BeginPlay
	(
		class ACAttachment* InAttachment, 
		class UCEquipment* InEquipment, 
		class ACharacter* InOwner, 
		const TArray<FDoActionData>& InDoActionData,
		const TArray<FHitData>& InHitData
	);

	virtual void Tick(float InDeltaTime) {};

public:
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

public:
	UFUNCTION()
		virtual void OnEquip() {}

	UFUNCTION()
		virtual void OnBeginEquip() {}

	UFUNCTION()
		virtual void OnEndEquip() {}

	UFUNCTION()
		virtual void OnUnequip() {}

public:
	UFUNCTION()
		virtual void OnAttachmentBeginCollision() {}

	UFUNCTION()
		virtual void OnAttachmentEndCollision() {}

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) {}

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) {}

protected:
	bool bInAction;
	bool bBeginAction;
	bool bEndEquip;

	class ACharacter* OwnerCharacter;
	class UWorld* World;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FDoActionData> DoActionDatas;
	TArray<FHitData> HitDatas;
};