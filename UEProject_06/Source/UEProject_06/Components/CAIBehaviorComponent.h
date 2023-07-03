#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait = 0, Approach, Action, Patrol, Hitted, Avoid, Dead, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEPROJECT_06_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Key")
		FName AIStateTypeKey = "AIStateType";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName TargetKey = "Target";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName LocationKey = "Location";

private:
	EAIStateType GetType();

public:
	bool IsWaitMode();
	bool IsApproachMode();
	bool IsActionMode();
	bool IsPatrolMode();
	bool IsHittedMode();
	bool IsAvoidMode();
	bool IsDeadMode();

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

public:	
	UCAIBehaviorComponent();

protected:
	virtual void BeginPlay() override;

public:
	class ACharacter* GetTarget();
	FVector GetLocation();

public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();
	void SetDeadMode();

private:
	void ChangeType(EAIStateType InType);

public:
	FAIStateTypeChanged OnAIStateTypeChanged;

private:
	class UBlackboardComponent* Blackboard;
		
};
