#include "Weapons/AddOns/CGhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACGhostTrail::ACGhostTrail()
{
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &Mesh, "Mesh");
}

void ACGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceConstant* material;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, 
		"MaterialInstanceConstant'/Game/Materials/M_GhostTrail_Inst.M_GhostTrail_Inst'");

	Material = UMaterialInstanceDynamic::Create(material, this);
	Material->SetVectorParameterValue("Color", Color);
	Material->SetScalarParameterValue("Exp", Exponent);

	Owner = Cast<ACharacter>(GetOwner());

	Mesh->SetVisibility(false);
	Mesh->SetSkeletalMesh(Owner->GetMesh()->SkeletalMesh);
	// 캡처를 미리 한번 진행하면 정확한 캡처가 일어난다 (원인은 잘 모르겠음..)
	Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());

	for (int32 i = 0; i < Owner->GetMesh()->SkeletalMesh->Materials.Num(); i++)
		Mesh->SetMaterial(i, Material);

	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([=]()
	{
		if (Mesh->IsVisible() == false)
			Mesh->ToggleVisibility();

		// 매 프레임마다의 Mesh 위치 초기화 및 고정
		float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		SetActorLocation(Owner->GetActorLocation() - FVector(0, 0, height));
		SetActorRotation(Owner->GetActorRotation() + FRotator(0, -90, 0));

		Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());
	});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, Interval, true, StartDelay);
}

void ACGhostTrail::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}


