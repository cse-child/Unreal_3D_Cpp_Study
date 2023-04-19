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
	// BP������
	// ApplyDamage�� Call�ϸ� TakeDamage�� Call�Ѵ�.
	// TakeDamage������ Recieve Damage�� ��? AnyDamage?? �Ӷ��

	FActionDamageEvent e;
	e.HitData = this; // this�� HitData�� ����

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
	CheckTrue(FMath::IsNearlyZero(StopTime)); // �������� ���� 0�̸� ���� �� �ʿ� X

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


