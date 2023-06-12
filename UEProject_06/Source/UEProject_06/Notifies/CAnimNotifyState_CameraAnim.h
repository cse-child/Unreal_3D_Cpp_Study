#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_CameraAnim.generated.h"


UCLASS()
class UEPROJECT_06_API UCAnimNotifyState_CameraAnim : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Camera")
		class UCameraAnim* CameraAnim;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float BlendInTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float BlendOutTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
		TSoftObjectPtr<class ACharacter> Target; // 애셋 참조

	UPROPERTY(EditAnywhere, Category = "Modifier")
		FVector LocationOffset = FVector(-400, 0, 90);

	UPROPERTY(EditAnywhere, Category = "Modifier")
		float PitchOffset = -15; // Pitch 회전값 보정

public:
	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

private:
	class UCCameraModifier* Modifier;
};
