// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CS330_FinalGameMode.h"
#include "CS330_FinalPawn.h"
#include "TopDownPlayerController.h"

ACS330_FinalGameMode::ACS330_FinalGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ACS330_FinalPawn::StaticClass();
	PlayerControllerClass = ATopDownPlayerController::StaticClass();
}

