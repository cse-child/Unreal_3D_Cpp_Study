#include "Weapons/SubActions/CSubAction_Sword.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/CAttachment.h"
#include "Components/CMovementComponent.h"
#include "Utilities/CHelpers.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/CGhostTrail.h"

void UCSubAction_Sword::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSubActionMode());

	Super::Pressed();

	State->SetActionMode();
	State->OnSubActionMode();

	GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);

	ActionData.DoAction(Owner);
}

void UCSubAction_Sword::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();

	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSubAction_Sword::OnAttachmentBeginOverlap);

	/* SubAction Skill 시작하면 바로 이동 */
	bMoving = true;

	Start = Owner->GetActorLocation();
	End = Start + Owner->GetActorForwardVector() * Distance;

	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FRotator rotation = Owner->GetActorRotation();

	/* 일섬 방향으로 이동하는 구간에 Overlap 되는 캐릭터의 충돌처리를 위한 Box Trace 처리*/
	TArray<AActor*> ignores; 
	ignores.Add(Owner); // Box 충돌에서 본인(Owner)은 빼기

	TArray<FHitResult> hitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> objects; // UEnum이 아닌 경우 Byte를 지정해줘야 한다.
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3); //ObjectTypeQuery3: Pawn / Pawn만 추적하기 위함

	UKismetSystemLibrary::BoxTraceMultiForObjects(Owner->GetWorld(), Start, End, FVector(0, radius, height), 
		rotation, objects, false, ignores, DrawDebug, hitResults, true);
	for(const FHitResult hitResult : hitResults)
	{
		ACharacter* character = Cast<ACharacter>(hitResult.GetActor());

		if(!!character)
		{
			// Box에 충돌된 캐릭터의 Collision을 새로만든 프로파일로 변경
			character->GetCapsuleComponent()->SetCollisionProfileName("Sword_SubAction");

			Overlapped.Add(character);
		}
	}

	/* 벽에 부딪혔을 때 벽에 박히지 않고 바깥에 정상적으로 멈출 수 있도록 처리 */
	FHitResult lineGHtResult;
	UKismetSystemLibrary::LineTraceSingle(Owner->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, 
		false, ignores, DrawDebug, lineGHtResult, true);
	// TraceTypeQuery1: Visibility 보이는것은 전부 충돌처리

	if(lineGHtResult.bBlockingHit)
	{
		FVector direction = (End - Start).GetSafeNormal2D();
		End = lineGHtResult.Location - (direction * radius * 2); // 충돌한 위치에서 캐릭터 크기 2배 만큼의 위치에 고정

		/* 벽에 충돌됐을 때 위치 조정 여부를 쉽게 판단하기 위해 Shpere 그리기 (디버깅용) */
		if (DrawDebug == EDrawDebugTrace::ForDuration)
			DrawDebugSphere(Owner->GetWorld(), End, radius * 2, 20, FColor::Magenta, true, 2);
	}

	/* 이동 방향, 거리를 확인하기 위한 Arrow 그리기 (디버깅용) */
	if (DrawDebug == EDrawDebugTrace::ForDuration)
		DrawDebugDirectionalArrow(Owner->GetWorld(), Start, End, 25, FColor::Green, true, 5, 0, 3);
}


void UCSubAction_Sword::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	bMoving = false; // Skill 이 끝나면 Moving 해제

	/* Sword_SubAction로 변경해두었던 Profile 복구 */
	for (ACharacter* character : Overlapped)
		character->GetCapsuleComponent()->SetCollisionProfileName("Pawn");

	GhostTrail->Destroy();
}


void UCSubAction_Sword::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
	CheckFalse(bMoving); // bMoving == true 라면 Actor 이동

	
	FVector location = Owner->GetActorLocation();
	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();

	// 끝지점(End)에 도달했다면 더이상 이동하지 않고 멈추기
	if(location.Equals(End, radius))
	{
		bMoving = false;
		Start = End = Owner->GetActorLocation();

		return;
	}

	// Start -> End 방향으로 위치를 더하면서 이동
	FVector direction = (End - Start).GetSafeNormal2D();

	// 두번째 매개변수 bSweep : 이동하며 Overlap 되는 객체들의 충돌을 검출해준다. bSweep을 켜면 HitResult를 사용할 수 있다.
	Owner->AddActorWorldOffset(direction * Speed * InDeltaTime, true);
}

void UCSubAction_Sword::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	HitData.SendDamage(Owner, InAttackCauser, InOther);
}
