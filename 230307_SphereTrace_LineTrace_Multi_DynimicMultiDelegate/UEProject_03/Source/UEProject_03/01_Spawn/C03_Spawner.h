#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C03_Spawner.generated.h"

UCLASS()
class UEPROJECT_03_API AC03_Spawner : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class AC02_Mesh> SpawnClasses[3];

public:	
	AC03_Spawner();

protected:
	virtual void BeginPlay() override;

};

