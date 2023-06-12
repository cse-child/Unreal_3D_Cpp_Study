#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CStaticMesh.generated.h"

UCLASS()
class UEPROJECT_05_API ACStaticMesh : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UMaterialInstanceConstant* Material;

public:	
	ACStaticMesh();

protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
public:
	void Paint();
#endif
};
