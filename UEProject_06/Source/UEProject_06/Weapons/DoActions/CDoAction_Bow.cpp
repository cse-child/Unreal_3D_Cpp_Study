#include "Weapons/DoActions/CDoAction_Bow.h"
#include "Global.h"
#include "Weapons/CAttachment.h"
#include "Weapons/AddOns/CProjectile.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

UCDoAction_Bow::UCDoAction_Bow()
{
	CHelpers::GetClass<ACProjectile>(&ArrowClass, "Blueprint'/Game/Weapons/Bow/BP_CProjectile_Arrow.BP_CProjectile_Arrow_C'");
}

void UCDoAction_Bow::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
	const TArray<FDoActionData>& InDoActionData, const TArray<FHitData>& InHitData)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionData, InHitData);

	Poseable = CHelpers::GetComponent<UPoseableMeshComponent>(InAttachment);
	OriginLocation = Poseable->GetBoneLocationByName("bow_string_mid", EBoneSpaces::ComponentSpace);
}

void UCDoAction_Bow::DoAction()
{
	CheckFalse(State->IsIdleMode());

	Super::DoAction();

	DoActionDatas[0].DoAction(OwnerCharacter);

	//FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	//GetAttachedArrow()->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow_Action");
}

void UCDoAction_Bow::Begin_DoAction()
{
	Super::Begin_DoAction();

	//ACProjectile* projectile = GetAttachedArrow();
	//projectile->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	//
	//projectile->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);
	//projectile->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay);
	//
	//projectile->Shoot(OwnerCharacter->GetActorForwardVector());

	//Poseable->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);
}

void UCDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();

	//CreateArrow();
}

void UCDoAction_Bow::CreateArrow()
{
	// World°¡ ³¡³µ´Ù¸é Return
	if (World->bIsTearingDown == true)
		return;

	FTransform transform;
	ACProjectile* arrow = World->SpawnActorDeferred<ACProjectile>(ArrowClass, transform, NULL, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	CheckNull(arrow);

	arrow->AddIgnoreActor(OwnerCharacter);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow");

	Arrows.Add(arrow);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}

ACProjectile* UCDoAction_Bow::GetAttachedArrow()
{
	for (ACProjectile* projectile : Arrows)
	{
		if (!!projectile->GetAttachParentActor())
			return projectile;
	}
	return NULL;
}

void UCDoAction_Bow::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
}

void UCDoAction_Bow::OnArrowEndPlay(ACProjectile* InDestroyer)
{
	Arrows.Remove(InDestroyer);
}

