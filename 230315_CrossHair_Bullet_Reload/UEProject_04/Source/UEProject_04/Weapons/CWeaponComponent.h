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

	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<class UCUserWidget_HUD> HUDClass;

public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsAR4Mode() { return Type == EWeaponType::AR4; }
	FORCEINLINE bool IsAK47Mode() { return Type == EWeaponType::AK47; }
	FORCEINLINE bool IsPistolMode() { return Type == EWeaponType::Pistol; }
public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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
	void Begin_Fire();
	void End_Fire();

public:
	void ToggleAutoFire();

public:
	bool IsInAim();
	FVector GetLeftHandLocation();

public:
	void Reload();
	void Eject_Magazine();	// źâ �и�
	void Spawn_Magazine();	// źâ ����
	void Load_Magazine();	// źâ ����
	void End_Reload();		// �Ϸ�

public:
	FWeaponTypeChanged OnWeaponTypeChanged;

private:
	EWeaponType Type = EWeaponType::Max;

private:
	class ACPlayer* Owner;
	TArray<class ACWeapon*> Weapons;
	class UCUserWidget_HUD* HUD;
};
