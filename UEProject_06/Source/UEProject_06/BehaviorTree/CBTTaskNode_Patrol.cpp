#include "BehaviorTree/CBTTaskNode_Patrol.h"
#include "Global.h"
#include "CPatrolPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	if (!!ai->GetPatrolPath())
	{
		Location = ai->GetPatrolPath()->GetMoveTo();

		return EBTNodeResult::InProgress;
	}

	FVector location = ai->GetActorLocation();

	// GetCurrent() : AI�� �ö��ִ� ��ġ�� �����ϴ� �׺���̼� �޽� ���
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	CheckNullResult(navSystem, EBTNodeResult::Failed);

	FNavLocation point(location);
	if (navSystem->GetRandomPointInNavigableRadius(location, Distance, point))
	{
		Location = point.Location;

		if (bDebugMode)
			DrawDebugSphere(ai->GetWorld(), Location, 25, 25, FColor::Green, true, 5);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	EPathFollowingRequestResult::Type result = controller->MoveToLocation(Location, AcceptanceDistance, false);

	switch (result)
	{
	case EPathFollowingRequestResult::Failed:
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		break;

	case EPathFollowingRequestResult::AlreadyAtGoal:
	{
		if (ai->GetPatrolPath())
			ai->GetPatrolPath()->UpdateIndex();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		break;
	}
	}
}
