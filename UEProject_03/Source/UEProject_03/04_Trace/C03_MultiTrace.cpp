#include "04_Trace/C03_MultiTrace.h"
#include "Global.h"
#include "Components/TextRenderComponent.h"

AC03_MultiTrace::AC03_MultiTrace()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");

	CreateTextRender();
}

void AC03_MultiTrace::BeginPlay()
{
	Super::BeginPlay();
	
}

void AC03_MultiTrace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector start = GetActorLocation();
	FVector start1 = FVector(start.X + 50, start.Y, start.Z);
	FVector start2 = FVector(start.X - 50, start.Y, start.Z);

	FVector end1 = start1 + GetActorForwardVector() * 600;
	FVector end2 = start2 + GetActorForwardVector() * 600;

	TArray<AActor*> ignores;
	TArray<FHitResult> hitResult1;
	TArray<FHitResult> hitResult2;

	// Multi 뒤에 아무것도 안붙으면 Channel
	UKismetSystemLibrary::LineTraceMulti(GetWorld(), start1, end1,
		ETraceTypeQuery::TraceTypeQuery1, false, ignores,
		EDrawDebugTrace::ForOneFrame, hitResult1, true);

	TArray<TEnumAsByte<EObjectTypeQuery>> types;
	types.Add(EObjectTypeQuery::ObjectTypeQuery1); // World Static

	UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), 
		start2, end2, types, false, ignores, 
		EDrawDebugTrace::ForOneFrame, hitResult2, true);

	TotalTime += DeltaTime;
	if(TotalTime >= 2.0f)
	{
		TotalTime = 0.0f;

		CLog::Log(FString::Printf(L"-- Channel : %d", hitResult1.Num()));
		for (const FHitResult& hitResult : hitResult1)
			CLog::Log(hitResult.GetActor()->GetName());

		CLog::Log(FString::Printf(L"-- Objects : %d", hitResult2.Num()));
		for (const FHitResult& hitResult : hitResult2)
			CLog::Log(hitResult.GetActor()->GetName());
	}
}



