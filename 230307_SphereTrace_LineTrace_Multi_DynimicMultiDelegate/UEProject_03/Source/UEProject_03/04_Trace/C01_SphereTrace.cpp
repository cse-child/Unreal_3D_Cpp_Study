#include "04_Trace/C01_SphereTrace.h"
#include "Global.h"
#include "Particles/ParticleSystemComponent.h"

AC01_SphereTrace::AC01_SphereTrace()
{
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle");

	UParticleSystem* particle;
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_hit_thunder.P_ky_hit_thunder'");

	Particle->SetTemplate(particle); // Template : 플레이어 할 파티클
	Particle->bAutoActivate = false;
	Particle->SetRelativeScale3D(FVector(2));
}

void AC01_SphereTrace::BeginPlay()
{
	Super::BeginPlay();
	
}

void AC01_SphereTrace::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	FVector location = GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> types;
	types.Add(EObjectTypeQuery::ObjectTypeQuery4);

	TArray<AActor*> ignores;
	TArray<FHitResult> hitResults;

	bool b = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), location, location, 
		300, types, false, ignores, DrawDebug, hitResults, true, 
		FLinearColor::Red, FLinearColor::Green, DrawTime);
	CheckFalse(b);

	Particle->ResetParticles(); // 끝까지가면 Active가 False로 변해있어서 사용할 때 Reset
	Particle->SetActive(true);

	// 복사생성자가 호출됨 -> 구조체라면 복사되는 메모리가 크기때문에 참조를 사용
	//for(FHitResult hitResult : hitResults)
	for(const FHitResult& hitResult : hitResults)
	{
		UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(hitResult.GetActor()->GetRootComponent());
		if (!!mesh && mesh->IsSimulatingPhysics())
			mesh->AddRadialImpulse(location, 1000, (MaxMass - mesh->GetMass()) * ImpulseAmount,
				ERadialImpulseFalloff::RIF_Linear);
	}
}


