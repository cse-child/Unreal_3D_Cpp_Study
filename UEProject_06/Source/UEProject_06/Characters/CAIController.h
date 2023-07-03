#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"


UCLASS()
class UEPROJECT_06_API ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACAIController();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	class ACEnemy_AI* Enemy;
	class UCAIBehaviorComponent* Behavior;
};


