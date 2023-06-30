#include "Characters/CEnemy.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CStatusComponent.h"
#include "Weapons/CWeaponStructures.h"

ACEnemy::ACEnemy()
{
	/* Component 眠啊 */
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");

	/* Mesh */
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	/* Anim Instance */
	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass< UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);

	/* Rotation Rate */
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0); // 糠吧澜龙
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	Movement->SetSpeed(ESpeedType::Run);
	Movement->DisableControlRotation();

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	/* Dynamic Material 积己 */
	for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); i++)
	{
		UMaterialInstanceDynamic* instance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterials()[i], this);
		GetMesh()->SetMaterial(i, instance);
	}

	Change_Color(this, OriginColor);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted:
		Hitted();
		break;

	case EStateType::Dead:
		Dead();
		break;
	}
}

/* 单固瘤 罐扁 */
float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetHittedMode();

	return damage;
}

void ACEnemy::Hitted()
{
	/* HP-Damage 贸府 */
	Status->Damage(Damage.Power);
	Damage.Power = 0;

	/* 荤噶贸府 */
	if(Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();

		return;
	}

	/* Hit Color 贸府 */
	Change_Color(this, FLinearColor::Red);

	FTimerDelegate timerDelegate;
	timerDelegate.BindUFunction(this, "RestoreColor");

	GetWorld()->GetTimerManager().SetTimer(RestoreColor_TimerHandle, timerDelegate, 0.2f, false);

	/* Hit Data 贸府 */
	if(!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;

		data->PlayMontage(this);
		data->PlayHitStop(GetWorld());
		data->PlaySoundWave(this);
		data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

		FVector start = GetActorLocation();
		FVector target = Damage.Character->GetActorLocation();
		FVector direction = target - start;
		direction.Normalize();

		LaunchCharacter(-direction * data->Launch, false, false);
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	}

	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;
}

void ACEnemy::End_Hitted()
{
	if (!!Damage.Event && !!Damage.Event->HitData)
		Damage.Event->HitData->EndHitted(this);

	State->SetIdleMode();
}

void ACEnemy::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Montages->PlayDeadMode();
}

void ACEnemy::End_Dead()
{
	Destroy();
}

void ACEnemy::RestoreColor()
{
	Change_Color(this, OriginColor);

	GetWorld()->GetTimerManager().ClearTimer(RestoreColor_TimerHandle);
}
