#include "Characters/CEnemy_AI.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"

ACEnemy_AI::ACEnemy_AI()
{
	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();
}
