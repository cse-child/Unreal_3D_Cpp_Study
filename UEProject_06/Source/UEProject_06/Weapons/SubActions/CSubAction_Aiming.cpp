#include "Weapons/SubActions/CSubAction_Aiming.h"

#include "AIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/CAttachment_Bow.h"


UCSubAction_Aiming::UCSubAction_Aiming()
{
	CHelpers::GetAsset<UCurveVector>(&Curve, "CurveVector'/Game/Weapons/Bow/Curve_Aiming.Curve_Aiming'");
}

void UCSubAction_Aiming::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);

	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnAiming");

	Timeline.AddInterpVector(Curve, timeline);
	Timeline.SetPlayRate(AimingSpeed);

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	if(!!bow)
		Bending = bow->GetBend();
	
}

void UCSubAction_Aiming::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Timeline.TickTimeline(InDeltaTime);
	//*Bending += InDeltaTime * 0.1f;
	//CLog::Print(*Bending, 1);
}

void UCSubAction_Aiming::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;
	*Bending = Output.Y;
}

void UCSubAction_Aiming::Pressed()
{
	CheckTrue(State->IsSubActionMode());

	/* EnemyÀÇ Bow Aiming */
	if(!!Owner->GetController<AAIController>())
	{
		Super::Pressed();
		State->OnSubActionMode();

		return;
	}

	CheckNull(SpringArm);
	CheckNull(Camera);

	Super::Pressed();

	State->OnSubActionMode();

	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	Timeline.PlayFromStart();
}

void UCSubAction_Aiming::Released()
{
	CheckFalse(State->IsSubActionMode());

	if (!!Owner->GetController<AAIController>())
	{
		Super::Released();
		State->OffSubActionMode();

		return;
	}

	CheckNull(SpringArm);
	CheckNull(Camera);

	Super::Released();

	State->OffSubActionMode();

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	Timeline.ReverseFromEnd();
}