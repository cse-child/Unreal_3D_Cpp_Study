#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class EXAMPLE_API FExampleMeshWindow
	:public FAssetEditorToolkit
{
public:
	static void OpenWindow(UObject* InAsset);
	static void Shutdown();

private:
	static TSharedPtr<FExampleMeshWindow> Instance;

private:
	void Open(UObject* InAsset);

public:
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

private:
	static const FName EditorName;
	static const FName ViewportTabId;
	static const FName DetailsTabId;
	static const FName PreviewTabId;
};
