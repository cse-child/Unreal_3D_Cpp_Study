#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Patrol.generated.h"

UCLASS()
class UEPROJECT_06_API UCBTTaskNode_Patrol : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Random")
		bool bDebugMode;

	UPROPERTY(EditAnywhere, Category = "Random")
		float AcceptanceDistance = 20;

	UPROPERTY(EditAnywhere, Category = "Random")
		float Distance = 1500;


public:
	UCBTTaskNode_Patrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

