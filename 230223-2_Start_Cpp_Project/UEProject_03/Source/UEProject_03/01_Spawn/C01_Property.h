#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C01_Property.generated.h"

UCLASS()
class UEPROJECT_03_API AC01_Property : public AActor
{
	GENERATED_BODY()
	
public:	
	AC01_Property();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
