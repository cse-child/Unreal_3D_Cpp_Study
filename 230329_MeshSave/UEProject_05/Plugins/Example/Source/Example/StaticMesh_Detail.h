#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

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


