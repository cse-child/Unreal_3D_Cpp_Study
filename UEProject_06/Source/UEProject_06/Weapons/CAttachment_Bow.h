#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Bow.generated.h"


UCLASS()
class UEPROJECT_06_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "View")
		FVector2D ViewPitchRange = FVector2D(-40, +30);

private:
	UPROPERTY(VisibleAnywhere)
		class UPoseableMeshComponent* Mesh;

public:
	ACAttachment_Bow();

protected:
	virtual void BeginPlay() override;

public:
	void OnBeginEquip_Implementation() override;
	void OnUnequip_Implementation() override;

private:
	FVector2D OriginViewPitchRange;
};

