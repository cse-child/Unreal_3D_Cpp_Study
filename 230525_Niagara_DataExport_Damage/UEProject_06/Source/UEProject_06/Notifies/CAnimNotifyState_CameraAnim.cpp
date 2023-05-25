#include "Notifies/CAnimNotifyState_CameraAnim.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraAnim.h"
#include "Weapons/AddOns/CCameraModifier.h"

FString UCAnimNotifyState_CameraAnim::GetNotifyName_Implementation() const
{
	return "CameraAnim";
}

void UCAnimNotifyState_CameraAnim::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	CheckNull(CameraAnim);

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	ACharacter* target = character;
	if (Target.IsNull() == false)
		target = Target.Get();

	APlayerController* controller = character->GetController<APlayerController>();
	CheckNull(character);

	/* 모티파이어 카메라 등록 */
	UCameraModifier* modifier = controller->PlayerCameraManager->AddNewCameraModifier(UCCameraModifier::StaticClass());
	Modifier = Cast<UCCameraModifier>(modifier);

	FVector location = target->GetActorLocation();

	// 회전만큼 카메라 움직여주기
	float yaw = controller->GetControlRotation().Yaw;
	location += FRotator(0, yaw, 0).RotateVector(LocationOffset);

	Modifier->SetLocation(location);
	Modifier->SetRotation(FRotator(PitchOffset, yaw, 0));


	controller->SetControlRotation(FRotator(0, controller->GetControlRotation().Yaw, 0));
	// UserDefined 일때만 각도를 설정 가능
	controller->PlayerCameraManager->PlayCameraAnim(CameraAnim, PlayRatio, 1, BlendInTime, BlendOutTime, 
		false, false, 0, ECameraShakePlaySpace::UserDefined, controller->GetControlRotation());
}

void UCAnimNotifyState_CameraAnim::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	CheckNull(CameraAnim);

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	APlayerController* controller = character->GetController<APlayerController>();
	CheckNull(controller);

	/* 모티파이어 카메라 제거 */
	controller->PlayerCameraManager->RemoveCameraModifier(Modifier);
}
