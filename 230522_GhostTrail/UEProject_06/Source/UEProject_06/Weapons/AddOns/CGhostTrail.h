#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGhostTrail.generated.h"

UCLASS()
class UEPROJECT_06_API ACGhostTrail : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGhostTrail();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		float StartDelay = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		float Interval = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		FLinearColor Color = FLinearColor(1, 1, 1, 1);

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		float Exponent = 1; // 색상이 안쪽에서 바깥쪽으로 퍼져나갈 값

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UPoseableMeshComponent* Mesh;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	class ACharacter* Owner;
	class UMaterialInstanceDynamic* Material;

	FTimerHandle TimerHandle;
};

