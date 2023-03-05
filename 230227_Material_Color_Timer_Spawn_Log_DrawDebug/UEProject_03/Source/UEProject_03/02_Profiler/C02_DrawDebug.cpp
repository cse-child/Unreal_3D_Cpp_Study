#include "02_Profiler/C02_DrawDebug.h"
#include "Global.h"

AC02_DrawDebug::AC02_DrawDebug()
{
	PrimaryActorTick.bCanEverTick = true;

	InitLocation[0] = FVector(0, 0, 0);
	InitLocation[1] = FVector(0, 1000, 0);
	InitLocation[2] = FVector(0, 500, 0);
	InitLocation[3] = FVector(0, 1500, 0);
	InitLocation[4] = FVector(500, 1000, 0);

	// 중점을 0으로 쓰기 위해 대칭으로 Min,Max 초기화
	Box = FBox(FVector(-50, -100, -50), FVector(50, 100, 50));

}

void AC02_DrawDebug::BeginPlay()
{
	Super::BeginPlay();
	
}

void AC02_DrawDebug::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 i = 0; i < 5; i++)
		Location[i] = InitLocation[i] + GetActorLocation();

	DrawDebugSolidBox(GetWorld(), Location[0] + Box.GetCenter(), Box.GetExtent(), FColor::Red);
	DrawDebugPoint(GetWorld(), Location[1], 100, FColor::Green);
	DrawDebugSphere(GetWorld(), Location[2], 100, 30, FColor::Blue);
	DrawDebugCircle(GetWorld(), Location[3], 100, 50, FColor::Magenta);
	DrawDebugLine(GetWorld(), Location[2], Location[3], FColor::Yellow, false, -1, 0, 10);

	FVector location = Location[2];

	location.X += 10;
	location.Y += 10;
	location.Z += FMath::Sin(GetWorld()->GetTimeSeconds() * 5.0f) * 400.0f;

	DrawDebugCapsule(GetWorld(), location, 200, 50, FQuat::Identity, FColor::White);
	DrawDebugDirectionalArrow(GetWorld(), Location[3], location, 400, FColor::Black, 
		false, -1, 0, 20);
}

