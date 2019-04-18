// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CS330_FinalGameMode.generated.h"

UCLASS(MinimalAPI)
class ACS330_FinalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACS330_FinalGameMode();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	bool TimeStopped;
};



