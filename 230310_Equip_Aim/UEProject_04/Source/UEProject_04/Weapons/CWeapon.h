#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeapon.generated.h"

USTRUCT()
struct FWeaponAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TargetArmLength;

	UPROPERTY(EditAnywhere)
		FVector SocketOffset;

	UPROPERTY(EditAnywhere)
		float FieldOfView;

public:
	void SetData(class ACharacter* InOwner);
	void SetDataByNoneCurve(class ACharacter* InOwner);
};

UCLASS(abstract)
class UEPROJECT_04_API ACWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		FName HolsterSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		class UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		float EquipMontage_PlayRate = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		FName RightHandSocketName;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Aim")
		FWeaponAimData BaseData;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
		FWeaponAimData AimData;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
		class UCurveFloat* AimCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
		float AimingSpeed = 200;

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

protected:
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* Mesh;

private:
	UPROPERTY(VisibleAnywhere)
		class UTimelineComponent* Timeline;

public:
	FORCEINLINE bool IsInAim() { return bInAim; }

public:	
	ACWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	bool CanEquip();
	void Equip();
	void Begin_Equip();
	void End_Equip();

public:
	bool CanUnequip();
	void Unequip();

public:
	bool CanAim();
	void Begin_Aim();
	void End_Aim();

private:
	UFUNCTION()
		void OnAiming(float Output);

private:
	bool bEquipping;
	bool bInAim;	// 조준
	bool bFiring;	// 쏘기
	bool bReload;	// 재장전

private:
	class ACPlayer* Owner;
};
