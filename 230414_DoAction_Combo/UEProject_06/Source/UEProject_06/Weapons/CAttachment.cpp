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
	// BP���� BeginPlay �ϱ����� Owner�� �ʱ�ȭ�ؾ� �������� ����
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	Super::BeginPlay(); // Blueprint���� Call
}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

