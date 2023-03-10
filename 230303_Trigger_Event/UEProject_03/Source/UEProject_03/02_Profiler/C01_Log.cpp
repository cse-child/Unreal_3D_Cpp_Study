#include "02_Profiler/C01_Log.h"
#include "Global.h"

AC01_Log::AC01_Log()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AC01_Log::BeginPlay()
{
	Super::BeginPlay();

	CLog::Log(10);
	CLog::Log(PI);
	CLog::Log("C01_Log");
	CLog::Log(GetActorLocation());
	CLog::Log(GetActorRotation());
	CLog::Log(this);

	CLog::Log(__FILE__);
	CLog::Log(__FUNCTION__);
	CLog::Log(__LINE__);

	LogLine();

	CLog::Print(10);
	CLog::Print(PI);
	CLog::Print("C01_Log");
	CLog::Print(GetActorLocation());
	CLog::Print(GetActorRotation());
	CLog::Print(this);
	PrintLine();
}

void AC01_Log::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TotalTime += DeltaTime;
	CLog::Print(TotalTime, 10);
}

