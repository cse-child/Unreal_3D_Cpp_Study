#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"

// 직렬화는 게임 오브젝트에서만 한다
struct FExampleDebuggerCategoryData
{
	bool bDraw;
	FString Name;
	FVector Location;
	FVector Forward;
};

class EXAMPLE_API FExampleDebuggerCategory
	:public FGameplayDebuggerCategory
{
public:
	FExampleDebuggerCategory();
	~FExampleDebuggerCategory();

public:
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

public:
	void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

private:
	FExampleDebuggerCategoryData PlayerPawnData;
	FExampleDebuggerCategoryData ForwardActorData;
	FExampleDebuggerCategoryData SelectActorData;

private:
	float TraceDistance = 500;
};


