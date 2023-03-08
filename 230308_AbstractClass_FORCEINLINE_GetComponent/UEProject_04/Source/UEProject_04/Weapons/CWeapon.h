#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeapon.generated.h"

UCLASS(abstract)
class UEPROJECT_04_API ACWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		FName HolsterSocketName;

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

protected:
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* Mesh;

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

private:
	class ACPlayer* Owner;
};
