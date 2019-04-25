// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIEnemyController.h"
#include "Engine.h"
#include "ProjectileEnemyController.generated.h"

enum EWizardAIState {
	EStart,
	EChase,
	EStop,
	EDead
};

/**
 * 
 */
UCLASS()
class CS330_FINAL_API AProjectileEnemyController : public AAIEnemyController
{
	GENERATED_BODY()

public:

	AProjectileEnemyController();

	void BeginPlay() override;
	void Possess(APawn * InPawn) override;
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result) override;

	

protected:
	APawn * WizardPawn;

	void SetState(EWizardAIState NewState);
	void HandleCurrentState(EWizardAIState NewState);

	EWizardAIState CurrentState;

	UPROPERTY(Category = AIBehavior, EditAnywhere, BlueprintReadWrite)
		float minRange;
	UPROPERTY(Category = AIBehavior, EditAnywhere, BlueprintReadWrite)
		float maxRange;
	UPROPERTY(Category = AIBehavior, EditAnywhere, BlueprintReadWrite)
		float noiseSeed;

	void Tick(float DeltaTime) override;
	
};
