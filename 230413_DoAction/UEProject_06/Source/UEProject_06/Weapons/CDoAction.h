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
	UCDoAction();

	virtual void BeginPlay
	(
		class ACAttachment* InAttachment, 
		class UCEquipment* InEquipment, 
		class ACharacter* InOwner, 
		const TArray<FDoActionData>& InDoActionData
	);

public:
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

protected:
	bool bBeginAction;

	class ACharacter* OwnerCharacter;
	class UWorld* World;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FDoActionData> DoActionDatas;
};



