#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Bow.generated.h"


UCLASS(Blueprintable)
class UEPROJECT_06_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACProjectile> ArrowClass;

public:
	FORCEINLINE void AttachString() { bAttachedString = true; } // End_BowString 노티파이에서 제어

public:
	UCDoAction_Bow();

	void BeginPlay
	(
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACharacter* InOwner,
		const TArray<FDoActionData>& InDoActionData,
		const TArray<FHitData>& InHitData
	) override;

	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	void Tick(float InDeltaTime) override;

	void OnEquip() override;
	void OnBeginEquip() override;
	void OnEndEquip() override;
	void OnUnequip() override;

private:
	void CreateArrow();
	class ACProjectile* GetAttachedArrow();

private:
	UFUNCTION()
		void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);

	UFUNCTION()
		void OnArrowEndPlay(class ACProjectile* InDestroyer);

private:
	class USkeletalMeshComponent* SkeletalMesh;
	class UPoseableMeshComponent* PoseableMesh;

private:
	TArray<class ACProjectile*> Arrows;
	FVector OriginLocation;

	bool bAttachedString = true;

private:
	float* Bending;
};


