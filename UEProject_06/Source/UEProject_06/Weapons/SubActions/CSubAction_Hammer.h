#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CSubAction_Hammer.generated.h"


UCLASS(Blueprintable)
class UEPROJECT_06_API UCSubAction_Hammer : public UCSubAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Aura")
		TSubclassOf<class ACAura> AuraClass;

	UPROPERTY(EditDefaultsOnly, Category = "Aura")
		FVector AuraLocation;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;

	UPROPERTY(EditAnywhere, Category = "Add-On")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

public:
	void Pressed() override;

	void Begin_SubAction_Implementation() override;
	void End_SubAction_Implementation() override;

private:
	class ACGhostTrail* GhostTrail;

};


