#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class UEPROJECT_06_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 2;

private:
	UPROPERTY(EditInstanceOnly, Category = "AI")
		TSoftObjectPtr<class ACPatrolPath> PatrolPath;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float LabelViewAmount = 3000;

private:
	UPROPERTY(EditDefaultsOnly)
		class UWidgetComponent* LabelWidget;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly)
		class UCAIBehaviorComponent* Behavior;

public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath.Get(); }

public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	void UpdateLabelRenderScale();
};