// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CS330_FinalGameMode.h"
#include "CS330_FinalPawn.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "TopDownPlayerController.h"

ACS330_FinalGameMode::ACS330_FinalGameMode()
{
	PlayerControllerClass = ATopDownPlayerController::StaticClass();
	// set default pawn class to our character class
	//DefaultPawnClass = ACS330_FinalPawn::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
}

