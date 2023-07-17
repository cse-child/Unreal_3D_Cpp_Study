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

	// ������ �����Ҵ� : CreateDefaultSubobject
	// ��Ÿ�� �����Ҵ�: NewObject
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600;
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 45;
	Sight->SetMaxAge(2);

	// BP���� ����� �� ������ ���
	Sight->DetectionByAffiliation.bDetectEnemies = true; // ���� ����
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight); // ���� ������ ����Ϸ��� �迭�� �����ؼ� �����ϱ�
	Perception->SetDominantSense(*Sight->GetSenseImplementation()); // ���� ���� ������ ��ϵ� ��� �켱����
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
	SetGenericTeamId(Enemy->GetTeamID()); // 0~255: 255�� �߸�

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
	// ù��° �Ű�����: � Sense�� �˻��� ���͸� ��ȯ����, nullptr�̸� �����ϴ� Sense ���λ��
	Perception->GetCurrentlyPerceivedActors(nullptr, actors); 

	if(actors.Num() > 0)
	{
		Blackboard->SetValueAsObject("Target", actors[0]);
		//CLog::Log(actors[0]->GetName());
		return;
	}

	Blackboard->SetValueAsObject("Target", nullptr);
}


