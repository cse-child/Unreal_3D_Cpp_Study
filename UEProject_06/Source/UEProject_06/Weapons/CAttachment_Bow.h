#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Bow.generated.h"


UCLASS()
class UEPROJECT_06_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class UPoseableMeshComponent* Mesh;

public:
	ACAttachment_Bow();

protected:
	virtual void BeginPlay() override;

public:
	void OnBeginEquip_Implementation();
	void OnUnequip_Implementation();
	
};
