// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileEnemyController.h"
#include "EnemyCharacter.h"
#include "CS330_FinalGameMode.h"

AProjectileEnemyController::AProjectileEnemyController()
{
	minRange = 600.0f;
	maxRange = 800.0f;
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

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(WizardPawn);

	switch (NewState)
	{
	case EChase:
		if (EnemyCharacter) {

		}
		break;
	case EStop:
		if (EnemyCharacter) {
			// MoveTo(EnemyCharacter->GetActorLocation());

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
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(WizardPawn);
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
			if (EnemyCharacter) {
				EnemyToPlayer.Normalize();
				EnemyCharacter->AddMovementInput(EnemyToPlayer, EnemyCharacter->MoveSpeed, false);
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
				if (EnemyCharacter) {
					EnemyToPlayer.Normalize();
					EnemyCharacter->AddMovementInput(-EnemyToPlayer, EnemyCharacter->MoveSpeed, false);
				}
			}
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
	ACS330_FinalGameMode* GameMode = Cast<ACS330_FinalGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode->TimeStopped) {
		HandleCurrentState(CurrentState);
	}
}
