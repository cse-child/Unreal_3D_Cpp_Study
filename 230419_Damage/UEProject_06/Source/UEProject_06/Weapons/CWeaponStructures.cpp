#include "Weapons/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Animation/AnimMontage.h"

void FDoActionData::DoAction(ACharacter* InOwner)
{
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOwner);

	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	if (!!movement)
		bCanMove ? movement->Move() : movement->Stop();

	if (!!InOwner && !!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);

	if (!!movement && bFixedCamera)
		movement->EnableFixedCamera();
}

///////////////////////////////////////////////////////////////////////////////

void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackerCauser, ACharacter* InOther)
{
	// BP에서는
	// ApplyDamage를 Call하면 TakeDamage를 Call한다.
	// TakeDamage에서는 Recieve Damage를 콜? AnyDamage?? 머라노

	FActionDamageEvent e;
	e.HitData = this; // this의 HitData를 전달

	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackerCauser);
}

void FHitData::PlayMontage(ACharacter* InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

	if (!!movement)
		bCanMove ? movement->Move() : movement->Stop();

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FHitData::PlayHitStop(UWorld* InWorld)
{
	CheckTrue(FMath::IsNearlyZero(StopTime)); // 오차값이 거의 0이면 수행 할 필요 X

	TArray<APawn*> pawns;
	for(AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		APawn* pawn = Cast<ACharacter>(actor);

		if(!!pawn)
		{
			pawn->CustomTimeDilation = 1e-3f;

			pawns.Add(pawn);
		}
	}

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
	{
		for (APawn* pawn : pawns)
			pawn->CustomTimeDilation = 1;
	});

	FTimerHandle timerHandle;
	InWorld->GetTimerManager().SetTimer(timerHandle, timerDelegate, StopTime, false);
}

void FHitData::EndHitted(ACharacter* InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

	if (!!movement)
		movement->Move();
}


