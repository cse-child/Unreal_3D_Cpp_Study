#include "Weapons/SubActions/CSubAction_Aiming.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"


UCSubAction_Aiming::UCSubAction_Aiming()
{
	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Weapons/Bow/Curve_Aim.Curve_Aim'");
}

void UCSubAction_Aiming::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);

	FOnTimelineFloat timeline;
	timeline.BindUFunction(this, "OnAiming");

	Timeline.AddInterpFloat(Curve, timeline);
	Timeline.SetPlayRate(AimingSpeed);
}

void UCSubAction_Aiming::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Timeline.TickTimeline(InDeltaTime);
}

void UCSubAction_Aiming::OnAiming(float Output)
{
	Camera->FieldOfView = Output;
}

void UCSubAction_Aiming::Pressed()
{
	CheckTrue(State->IsSubActionMode());
	CheckNull(SpringArm);
	CheckNull(Camera);
	CheckTrue(bAiming);

	Super::Pressed();

	State->OnSubActionMode();
	bAiming = true;

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
	CheckNull(SpringArm);
	CheckNull(Camera);
	CheckFalse(bAiming);

	Super::Released();

	State->OffSubActionMode();
	bAiming = false;

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	Timeline.ReverseFromEnd();
}