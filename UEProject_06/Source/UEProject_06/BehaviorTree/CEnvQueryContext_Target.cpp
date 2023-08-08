#include "BehaviorTree/CEnvQueryContext_Target.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UCEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	ACAIController* controller = ai->GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	AActor* target = Cast<AActor>(blackboard->GetValueAsObject("Target"));

	// Actor�� ��ȯ�ҰŸ� UEnvQueryItemType_Actor �� ���, Vector�� ��ȯ�Ϸ��� _VectorBase ���
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);

}
