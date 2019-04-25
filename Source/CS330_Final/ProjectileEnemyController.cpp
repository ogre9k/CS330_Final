// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileEnemyController.h"
#include "WizardCharacter.h"

AProjectileEnemyController::AProjectileEnemyController()
{
	minRange = 600.0f;
	maxRange = 800.0f;
	noiseSeed = FMath::RandRange(0.0f, 1.0f);

}

void AProjectileEnemyController::BeginPlay()
{
	Super::BeginPlay();
	CurrentState = EWizardAIState::EStart;
}

void AProjectileEnemyController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	WizardPawn = InPawn;
}

void AProjectileEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
}

void AProjectileEnemyController::SetState(EWizardAIState NewState)
{
	CurrentState = NewState;

	AWizardCharacter* WizardCharacter = Cast<AWizardCharacter>(WizardPawn);

	switch (NewState)
	{
	case EChase:
		if (WizardCharacter) {

		}
		break;
	case EStop:
		if (WizardCharacter) {
			// MoveTo(WizardCharacter->GetActorLocation());

		}
		break;
	case EDead:
		break;
	default:
		break;
	}
}

void AProjectileEnemyController::HandleCurrentState(EWizardAIState NewState)
{
	APawn * Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AWizardCharacter* WizardCharacter = Cast<AWizardCharacter>(WizardPawn);
	if (Player && WizardPawn) {
		FVector EnemyToPlayer = Player->GetActorLocation() - WizardPawn->GetActorLocation();
		float dist = EnemyToPlayer.Size();
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), dist));
		}
	}
	switch (NewState)
	{
	case EStart:
		SetState(EWizardAIState::EChase);
		break;
	case EChase:
		if (Player && WizardPawn) {
			FVector EnemyToPlayer = Player->GetActorLocation() - WizardPawn->GetActorLocation();
			float dist = EnemyToPlayer.Size();
			if (dist <= minRange)
				SetState(EWizardAIState::EStop);
			if (WizardCharacter) {
				EnemyToPlayer.Normalize();
				WizardCharacter->AddMovementInput(EnemyToPlayer, WizardCharacter->MoveSpeed, false);
			}
			
		}
		break;
	case EStop:
		if (Player && WizardPawn) {
			FVector EnemyToPlayer = Player->GetActorLocation() - WizardPawn->GetActorLocation();
			float dist = EnemyToPlayer.Size();
			if (dist > maxRange)
				SetState(EWizardAIState::EChase);
			else if (dist < minRange) {
				if (WizardCharacter) {
					EnemyToPlayer.Normalize();
					WizardCharacter->AddMovementInput(-EnemyToPlayer, WizardCharacter->MoveSpeed, false);
				}
			}
			FVector HorizontalMotion = WizardCharacter->GetActorRightVector();
			HorizontalMotion = HorizontalMotion * FMath::PerlinNoise1D(noiseSeed);
			WizardCharacter->AddMovementInput(HorizontalMotion, WizardCharacter->MoveSpeed / 2, false);
		}
		break;
	case EDead:

		break;
	default:
		break;
	}
}

void AProjectileEnemyController::Tick(float DeltaTime)
{
	noiseSeed += 0.01;
	HandleCurrentState(CurrentState);
}
