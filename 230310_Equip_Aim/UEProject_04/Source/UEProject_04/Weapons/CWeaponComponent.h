#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AR4, AK47, Pistol, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UEPROJECT_04_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
		TArray<TSubclassOf<class ACWeapon>> WeaponClasses;

public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsAR4Mode() { return Type == EWeaponType::AR4; }
	FORCEINLINE bool IsAK47Mode() { return Type == EWeaponType::AK47; }
	FORCEINLINE bool IsPistolMode() { return Type == EWeaponType::Pistol; }
public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

private:
	class ACWeapon* GetCurrentWeapon();

public:
	void SetUnarmedMode();
	void SetAR4Mode();

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	void Begin_Equip();
	void End_Equip();

public:
	void Begin_Aim();
	void End_Aim();

public:
	bool IsInAim();

public:
	FWeaponTypeChanged OnWeaponTypeChanged;

private:
	EWeaponType Type = EWeaponType::Max;

private:
	class ACPlayer* Owner;
	TArray<class ACWeapon*> Weapons;
};
