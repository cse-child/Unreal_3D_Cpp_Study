#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

void UCDoAction_Combo::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1);

	if(bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());

	Super::DoAction(); // SetActionMode
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;

	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	DoActionDatas[Index].PlayEffect(OwnerCharacter->GetWorld(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());
	Index = 0;
}

void UCDoAction_Combo::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();

	if(DoActionDatas[Index].bFixedCamera == false)
	{
		Hitted.Empty();

		return;
	}

	float angle = -2.0f; // 내적(-1~1) 범위에 따라 -2는 캐릭터가 없을 때
	ACharacter* candidate = nullptr;

	for(ACharacter* hitted : Hitted)
	{
		// Player -> Enemy 방향벡터 구하기
		FVector direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
		direction = direction.GetSafeNormal2D();

		FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();

		// 내적 최댓값 찾기 => Player Forward에 가장 근접한 Enemy 찾기
		float dot = FVector::DotProduct(direction, forward);
		if(dot >= angle) 
		{
			angle = dot;
			candidate = hitted;
		}
	}

	if(!!candidate)
	{
		// Player->Enemy 바라보도록 Camera(AController) 방향 돌리기
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());
		FRotator target = FRotator(0, rotator.Yaw, 0); // (Pitch,Yaw,Roll)

		AController* controller = OwnerCharacter->GetController<AController>();
		controller->SetControlRotation(target);
	}
	Hitted.Empty();
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
	CheckNull(InOther);

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	Hitted.AddUnique(InOther);

	CheckTrue(HitDatas.Num() < 1);
	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);
}