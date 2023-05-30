#include "Weapons/DoActions/CDoAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"


UCDoAction_Warp::UCDoAction_Warp()
{
}

void UCDoAction_Warp::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
                                const TArray<FDoActionData>& InDoActionData, const TArray<FHitData>& InHitData)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionData, InHitData);

	Decal = CHelpers::GetComponent<UDecalComponent>(InAttachment);
	PlayerController = OwnerCharacter->GetController<APlayerController>();

	bEquipped = InEquipment->GetEquipped(); // Equipment 객체는 같은 선상에 있는 객체이므로 보유하면 안된다.
	// 따라서 bEquipped 변수의 값만 참조하기 위해 const bool* 형으로 변수를 가져오는 것
}

void UCDoAction_Warp::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	FVector location = FVector::ZeroVector;
	FRotator rotation = FRotator::ZeroRotator;

	if(GetCursorLocationAndRotation(location, rotation) == false)
	{
		Decal->SetVisibility(false);

		return;
	}

	Decal->SetVisibility(true);

	if(*bEquipped)
	{
		Decal->SetWorldLocation(location);
		Decal->SetWorldRotation(rotation);
	}
}

void UCDoAction_Warp::DoAction()
{
	CheckFalse(DoActionDatas.Num() > 0);
	CheckFalse(State->IsIdleMode());

	Super::DoAction();

	FRotator rotation;
	if (GetCursorLocationAndRotation(MoveToLocation, rotation))
	{
		float height = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		MoveToLocation = FVector(MoveToLocation.X, MoveToLocation.Y, MoveToLocation.Z + height);

		float yaw = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), MoveToLocation).Yaw;
		OwnerCharacter->SetActorRotation(FRotator(0, yaw, 0));
	}
	else
		return;

	DoActionDatas[0].DoAction(OwnerCharacter);
}

void UCDoAction_Warp::Begin_DoAction()
{
	//Super::Begin_DoAction();

	OwnerCharacter->SetActorLocation(MoveToLocation);
	MoveToLocation = FVector::ZeroVector;
}

bool UCDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotator)
{
	CheckNullResult(PlayerController, false);

	FHitResult hitResult;
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);
	CheckFalseResult(hitResult.bBlockingHit, false);

	OutLocation = hitResult.Location;
	OutRotator = hitResult.ImpactNormal.Rotation();

	return true;
}
