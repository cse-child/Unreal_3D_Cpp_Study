#include "Weapons/AddOns/CThornObject.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"

ACThornObject::ACThornObject()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Root);

}

void ACThornObject::BeginPlay()
{
	Super::BeginPlay();

	Niagara->SetNiagaraVariableObject("Collision", this);
	Niagara->OnSystemFinished.AddDynamic(this, &ACThornObject::OnSystemFinished);

	if (!!NiagaraMesh)
	{
		FBox box = NiagaraMesh->GetBoundingBox();
		BoxExtent = (box.Max - box.Min).GetAbs() * 0.5f;
	}

}

void ACThornObject::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data,
	UNiagaraSystem* NiagaraSystem)
{
	CheckFalse(Data.Num() > 0);

	//반복되는 함수들은 계속 할당, 삭제를 반복하지 않기 위해 static으로 선언
	//스태틱은 최초 한번만 할당
	static TArray<AActor*> ignores;
	ignores.AddUnique(GetOwner());

	static FHitResult hitResult;
	for (int32 i = Data.Num() - 1; i >= 0; i--)
	{
		FVector position = Data[i].Position + GetActorLocation();
		FVector scale = Data[i].Velocity * BoxExtent;

		UKismetSystemLibrary::BoxTraceSingleByProfile(GetWorld(), position, position, scale, NiagaraMeshRotation, "Pawn", false, ignores, EDrawDebugTrace::None, hitResult, true);
		if (hitResult.bBlockingHit)
		{
			if (!!CollisionEffect)
			{
				FTransform transform = CollisionEffectTransform;
				transform.AddToTranslation(hitResult.Location);

				CHelpers::PlayEffect(GetWorld(), CollisionEffect, transform);
			}

			ACharacter* character = Cast<ACharacter>(hitResult.GetActor());
			if (!!character && character != GetOwner())
			{
				HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, character);
			}
		}

	}

}

void ACThornObject::OnSystemFinished(UNiagaraComponent* PSystem)
{
	Destroy();
}

