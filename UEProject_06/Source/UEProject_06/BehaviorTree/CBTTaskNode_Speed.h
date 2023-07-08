#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CMovementComponent.h"
#include "CBTTaskNode_Speed.generated.h"


UCLASS()
class UEPROJECT_06_API UCBTTaskNode_Speed : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Type")
		ESpeedType Type;

private:
	UCBTTaskNode_Speed();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};


