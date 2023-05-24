#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponStructures.h"
#include "CAura.generated.h"

UCLASS()
class UEPROJECT_06_API ACAura : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData HitData;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DamageInterval = 0.1f;

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* Niagara;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Box;

public:	
	ACAura();

protected:
	virtual void BeginPlay() override;

private:
	TArray<class ACharacter*> Hitted;
	FTimerHandle TimerHandle;
};


