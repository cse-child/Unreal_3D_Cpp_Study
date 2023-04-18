#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Fist, OneHand, TwoHand, Warp, Rotator, Bow, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEPROJECT_06_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsFistMode() { return Type == EWeaponType::Fist; }
	FORCEINLINE bool IsOneHandMode() { return Type == EWeaponType::OneHand; }
	FORCEINLINE bool IsTwoHandMode() { return Type == EWeaponType::TwoHand; }
	FORCEINLINE bool IsWarpMode() { return Type == EWeaponType::Warp; }
	FORCEINLINE bool IsRotatorMode() { return Type == EWeaponType::Warp; }
	FORCEINLINE bool IsBowMode() { return Type == EWeaponType::Bow; }

public:	
	UCWeaponComponent();

private:
	bool IsIdleMode();

public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetUnarmedMode();
	void SetFistMode();
	void SetOneHandMode();
	void SetTwoHandMode();
	void SetWarpMode();
	void SetRotatorMode();
	void SetBowMode();

	void DoAction();

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	FWeaponTypeChanged OnWeaponTypeChange;

private:
	class ACharacter* OwnerCharacter;

	EWeaponType Type = EWeaponType::Max;
};


