#include "Characters/CAIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ACAIController::ACAIController()
{
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");

	// 생성자 동적할당 : CreateDefaultSubobject
	// 런타임 동적할당: NewObject
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600;
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 45;
	Sight->SetMaxAge(2);

	// BP에선 사용할 수 없었던 기능
	Sight->DetectionByAffiliation.bDetectEnemies = true; // 적만 감지
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight); // 여러 감지를 사용하려면 배열로 저장해서 전달하기
	Perception->SetDominantSense(*Sight->GetSenseImplementation()); // 여러 개의 감지가 등록된 경우 우선순위
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);
	SetGenericTeamId(Enemy->GetTeamID()); // 0~255: 255는 중립

	CheckNull(Enemy->GetBehaviorTree());
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);
	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(Enemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	// 첫번째 매개변수: 어떤 Sense로 검색된 액터를 반환할지, nullptr이면 존재하는 Sense 전부사용
	Perception->GetCurrentlyPerceivedActors(nullptr, actors); 

	if(actors.Num() > 0)
	{
		Blackboard->SetValueAsObject("Target", actors[0]);
		//CLog::Log(actors[0]->GetName());
		return;
	}

	Blackboard->SetValueAsObject("Target", nullptr);
}


