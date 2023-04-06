#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"

// ����ȭ�� ���� ������Ʈ������ �Ѵ�
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


