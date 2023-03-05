#pragma once

#include "CoreMinimal.h"

class UEPROJECT_03_API CHelpers
{
public:
	template<typename T>
	static void CreateComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
		InActor->SetRootComponent(*OutComponent);
	}

	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		*OutObject = Cast<T>(StaticFindObject(T::StaticClass(), nullptr, *InPath));
	}
};