#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

// TCommands<ButtonCommand> : �ڱ� �ڽ��� ���ø����� �����Ͽ� �̱������� ���

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




