#include "Characters/CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/SubActions/CSubAction_Aiming.h"
#include "Parkour/CParkourComponent.h"
#include "Components/CFeetComponent.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	if (!!Weapon)
		Weapon->OnWeaponTypeChange.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

	/* 애니메이션 튀는 현상 고치기 위한 코드 */
	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);

	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();

	CheckNull(Weapon);
	if (!!Weapon->GetSubAction())
	{
		bBow_Aiming = true;
		bBow_Aiming &= WeaponType == EWeaponType::Bow;
		bBow_Aiming &= Weapon->GetSubAction()->GetInAction();
	}

	/* 수업이니까 Unarmed 모드에만 IK가 적용되도록 제한함 */
	CheckFalse(Weapon->IsUnarmedMode());

	/* Parkour 도중에는 IK를 적용되지 않도록 함 */
	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(OwnerCharacter);
	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(OwnerCharacter);

	bFeet = false;
	if(!!parkour && !!feet)
	{
		bFeet = parkour->IsExecuting() == false;
		FeetData = feet->GetData();
	}
	else if(!!feet) // Parkour는 할당X, Feet만 할당O => Enemy
	{
		bFeet = true;
		FeetData = feet->GetData();
	}
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}

