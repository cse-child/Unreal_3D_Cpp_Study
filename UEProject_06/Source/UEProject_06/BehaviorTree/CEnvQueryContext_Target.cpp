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

	// Actor로 반환할거면 UEnvQueryItemType_Actor 를 사용, Vector로 반환하려면 _VectorBase 사용
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);

}
