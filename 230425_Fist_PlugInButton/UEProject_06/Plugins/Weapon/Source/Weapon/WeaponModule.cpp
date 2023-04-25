#include "WeaponModule.h"
#include "WeaponStyle.h"
#include "WeaponCommand.h"

#define LOCTEXT_NAMESPACE "FWeaponModule"

IMPLEMENT_MODULE(FWeaponModule, Weapon)

void FWeaponModule::StartupModule()
{
	FWeaponStyle::Get();

	Command = MakeShareable(new FWeaponCommand());
	Command->Startup();
}

void FWeaponModule::ShutdownModule()
{
	if (Command.IsValid())
		Command.Reset();
}

#undef LOCTEXT_NAMESPACE