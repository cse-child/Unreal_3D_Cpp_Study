// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEProject_05GameMode.h"
#include "UEProject_05PlayerController.h"
#include "UEProject_05Character.h"
#include "UObject/ConstructorHelpers.h"

AUEProject_05GameMode::AUEProject_05GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AUEProject_05PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}