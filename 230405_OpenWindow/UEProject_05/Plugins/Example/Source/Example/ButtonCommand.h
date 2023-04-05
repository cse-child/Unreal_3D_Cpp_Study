#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

// TCommands<ButtonCommand> : 자기 자신을 템플릿으로 전달하여 싱글톤으로 사용

class EXAMPLE_API FButtonCommand
	:public TCommands<FButtonCommand>
{
public:
	FButtonCommand();
	~FButtonCommand();

public:
	void RegisterCommands() override;

public:
	TSharedPtr<FUICommandList> Command;

public:
	TSharedPtr<FUICommandInfo> LoadMesh;
	TSharedPtr<FUICommandInfo> OpenWindow;

private:
	void OnClicked_LoadMesh();
	void OnClicked_OpenWindow();
};




