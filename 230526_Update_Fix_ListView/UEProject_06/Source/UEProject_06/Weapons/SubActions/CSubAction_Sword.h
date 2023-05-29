#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CSubAction_Sword.generated.h"


UCLASS(Blueprintable)
class UEPROJECT_06_API UCSubAction_Sword : public UCSubAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float Distance = 1000;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float Speed = 200;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FHitData HitData;

private:
	UPROPERTY(EditAnywhere, Category = "Add-On")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

public:
	void Pressed() override;
	void Begin_SubAction_Implementation() override;
	void End_SubAction_Implementation() override;
	auto Tick_Implementation(float InDeltaTime) -> void override;

private:
	UFUNCTION()
		void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);

private:
	bool bMoving;		// 이동중인가?

	FVector Start;		// 공격 시작 지점
	FVector End;		// 공격 끝 지점

	TArray<class ACharacter*> Overlapped;

private:
	class ACGhostTrail* GhostTrail;
};

