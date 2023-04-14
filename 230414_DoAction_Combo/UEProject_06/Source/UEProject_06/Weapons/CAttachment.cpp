#include "Weapons/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"

ACAttachment::ACAttachment()
{
	CHelpers::CreateComponent(this, &Root, "Root");
}

void ACAttachment::BeginPlay()
{
	// BP에서 BeginPlay 하기전에 Owner를 초기화해야 오류나지 않음
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	Super::BeginPlay(); // Blueprint에서 Call
}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

