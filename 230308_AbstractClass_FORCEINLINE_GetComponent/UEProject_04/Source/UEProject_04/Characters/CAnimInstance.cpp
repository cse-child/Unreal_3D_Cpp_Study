#include "CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

	//UActorComponent* comp = OwnerCharacter->GetComponentByClass(UCWeaponComponent::StaticClass());
	//Weapon = Cast<UCWeaponComponent>(comp);

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(Weapon);

	Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}


