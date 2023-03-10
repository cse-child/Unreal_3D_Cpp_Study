#include "03_Collision/C01_ActorOverlap.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

AC01_ActorOverlap::AC01_ActorOverlap()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);

	CreateTextRender();

	Box->bHiddenInGame = false;
	Box->SetRelativeScale3D(FVector(3));
}

void AC01_ActorOverlap::BeginPlay()
{
	Super::BeginPlay();

	//OnActorBeginOverlap.AddDynamic(this, &AC01_ActorOverlap::BeginOverlap);
	//OnActorEndOverlap.AddDynamic(this, &AC01_ActorOverlap::EndOverlap);
}

//void AC01_ActorOverlap::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
//{
//}
//
//void AC01_ActorOverlap::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
//{
//}

