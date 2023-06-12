#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

// TCommands<ButtonCommand> : 자기 자신을 템플릿으로 전달하여 싱글톤으로 사용

class WEAPON_API FWeaponCommand
	:public TCommands<FWeaponCommand>
{
public:
	FWeaponCommand();
	~FWeaponCommand();

	void Startup();

public:
	void RegisterCommands() override;

private:
	TSharedPtr<FExtender> Extender;

	TSharedPtr<FUICommandList> Command;
	TSharedPtr<FUICommandInfo> Id;

private:
	void AddToolBar(FToolBarBuilder& InBuilder);
	void OnClicked();
};




