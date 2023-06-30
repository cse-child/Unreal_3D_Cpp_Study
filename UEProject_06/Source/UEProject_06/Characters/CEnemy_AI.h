#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class UEPROJECT_06_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponComponent* Weapon;

public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override;
};
