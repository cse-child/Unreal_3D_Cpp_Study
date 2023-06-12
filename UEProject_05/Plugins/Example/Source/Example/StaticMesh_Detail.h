#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

struct FStaticMesh_DetailData
{
	TArray<FVector> Positions;
	TArray<FVector> Normals;
	TArray<FColor> Colors;
	TArray<FVector2D> Uvs;
	TArray<int32> Indices;

	FVector Extent;
	float Radius;

	friend FArchive& operator <<(FArchive& InArchive, FStaticMesh_DetailData& InData)
	{
		return InArchive
			<< InData.Positions
			<< InData.Normals
			<< InData.Uvs
			<< InData.Colors
			<< InData.Indices
			<< InData.Extent
			<< InData.Radius;
	}
};

class EXAMPLE_API FStaticMesh_Detail
	:public IDetailCustomization
{
public:
	static TSharedRef<class IDetailCustomization> MakeInstance();

	void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	FReply OnClicked_Paint();
	FReply OnClicked_Save();

private:
	TArray<TWeakObjectPtr<UObject>> Objects;
};


