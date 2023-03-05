#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C02_Mesh.generated.h"

UCLASS()
class UEPROJECT_03_API AC02_Mesh : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

public:	
	AC02_Mesh();

protected:
	virtual void BeginPlay() override;

private:
	class UMaterialInstanceDynamic* Material;
};
