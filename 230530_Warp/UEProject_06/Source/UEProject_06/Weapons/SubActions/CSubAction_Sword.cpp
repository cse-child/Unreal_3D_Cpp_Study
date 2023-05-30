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

	/* SubAction Skill �����ϸ� �ٷ� �̵� */
	bMoving = true;

	Start = Owner->GetActorLocation();
	End = Start + Owner->GetActorForwardVector() * Distance;

	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FRotator rotation = Owner->GetActorRotation();

	/* �ϼ� �������� �̵��ϴ� ������ Overlap �Ǵ� ĳ������ �浹ó���� ���� Box Trace ó��*/
	TArray<AActor*> ignores; 
	ignores.Add(Owner); // Box �浹���� ����(Owner)�� ����

	TArray<FHitResult> hitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> objects; // UEnum�� �ƴ� ��� Byte�� ��������� �Ѵ�.
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3); //ObjectTypeQuery3: Pawn / Pawn�� �����ϱ� ����

	UKismetSystemLibrary::BoxTraceMultiForObjects(Owner->GetWorld(), Start, End, FVector(0, radius, height), 
		rotation, objects, false, ignores, DrawDebug, hitResults, true);
	for(const FHitResult hitResult : hitResults)
	{
		ACharacter* character = Cast<ACharacter>(hitResult.GetActor());

		if(!!character)
		{
			// Box�� �浹�� ĳ������ Collision�� ���θ��� �������Ϸ� ����
			character->GetCapsuleComponent()->SetCollisionProfileName("Sword_SubAction");

			Overlapped.Add(character);
		}
	}

	/* ���� �ε����� �� ���� ������ �ʰ� �ٱ��� ���������� ���� �� �ֵ��� ó�� */
	FHitResult lineGHtResult;
	UKismetSystemLibrary::LineTraceSingle(Owner->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, 
		false, ignores, DrawDebug, lineGHtResult, true);
	// TraceTypeQuery1: Visibility ���̴°��� ���� �浹ó��

	if(lineGHtResult.bBlockingHit)
	{
		FVector direction = (End - Start).GetSafeNormal2D();
		End = lineGHtResult.Location - (direction * radius * 2); // �浹�� ��ġ���� ĳ���� ũ�� 2�� ��ŭ�� ��ġ�� ����

		/* ���� �浹���� �� ��ġ ���� ���θ� ���� �Ǵ��ϱ� ���� Shpere �׸��� (������) */
		if (DrawDebug == EDrawDebugTrace::ForDuration)
			DrawDebugSphere(Owner->GetWorld(), End, radius * 2, 20, FColor::Magenta, true, 2);
	}

	/* �̵� ����, �Ÿ��� Ȯ���ϱ� ���� Arrow �׸��� (������) */
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

	bMoving = false; // Skill �� ������ Moving ����

	/* Sword_SubAction�� �����صξ��� Profile ���� */
	for (ACharacter* character : Overlapped)
		character->GetCapsuleComponent()->SetCollisionProfileName("Pawn");

	GhostTrail->Destroy();
}


void UCSubAction_Sword::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
	CheckFalse(bMoving); // bMoving == true ��� Actor �̵�

	
	FVector location = Owner->GetActorLocation();
	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();

	// ������(End)�� �����ߴٸ� ���̻� �̵����� �ʰ� ���߱�
	if(location.Equals(End, radius))
	{
		bMoving = false;
		Start = End = Owner->GetActorLocation();

		return;
	}

	// Start -> End �������� ��ġ�� ���ϸ鼭 �̵�
	FVector direction = (End - Start).GetSafeNormal2D();

	// �ι�° �Ű����� bSweep : �̵��ϸ� Overlap �Ǵ� ��ü���� �浹�� �������ش�. bSweep�� �Ѹ� HitResult�� ����� �� �ִ�.
	Owner->AddActorWorldOffset(direction * Speed * InDeltaTime, true);
}

void UCSubAction_Sword::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	HitData.SendDamage(Owner, InAttackCauser, InOther);
}
