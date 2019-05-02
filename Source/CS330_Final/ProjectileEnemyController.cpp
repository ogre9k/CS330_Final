// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileEnemyController.h"
#include "EnemyCharacter.h"
#include "CS330_FinalGameMode.h"

AProjectileEnemyController::AProjectileEnemyController()
{
	minRange = 600.0f;
	maxRange = 800.0f;
	noiseSeed = FMath::RandRange(0.0f, 1.0f);
}

void AProjectileEnemyController::BeginPlay()
{
	Super::BeginPlay();
	CurrentState = EAIState::EStart;
}

void AProjectileEnemyController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	EnemyPawn = InPawn;
}

void AProjectileEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
}

void AProjectileEnemyController::SetState(EAIState NewState)
{
	CurrentState = NewState;

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(EnemyPawn);

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

void AProjectileEnemyController::HandleCurrentState(EAIState NewState)
{
	APawn * Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(EnemyPawn);
	if (Player && EnemyPawn) {
		FVector EnemyToPlayer = Player->GetActorLocation() - EnemyPawn->GetActorLocation();
		float dist = EnemyToPlayer.Size();
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), dist));
		}
	}
	switch (NewState)
	{
	case EStart:
		SetState(EAIState::EChase);
		break;
	case EChase:
		if (Player && EnemyPawn) {
			FVector EnemyToPlayer = Player->GetActorLocation() - EnemyPawn->GetActorLocation();
			float dist = EnemyToPlayer.Size();
			if (dist <= minRange)
				SetState(EAIState::EStop);
			if (EnemyCharacter) {
				EnemyToPlayer.Normalize();
				EnemyCharacter->AddMovementInput(EnemyToPlayer, EnemyCharacter->MoveSpeed, false);
			}
			
		}
		break;
	case EStop:
		if (Player && EnemyPawn) {
			FVector EnemyToPlayer = Player->GetActorLocation() - EnemyPawn->GetActorLocation();
			float dist = EnemyToPlayer.Size();
			if (dist > maxRange)
				SetState(EAIState::EChase);
			else if (dist < minRange) {
				if (EnemyCharacter) {
					EnemyToPlayer.Normalize();
					EnemyCharacter->AddMovementInput(-EnemyToPlayer, EnemyCharacter->MoveSpeed, false);
				}
			}
			FVector HorizontalMotion = EnemyCharacter->GetActorRightVector();
			HorizontalMotion = HorizontalMotion * FMath::PerlinNoise1D(noiseSeed);
			EnemyCharacter->AddMovementInput(HorizontalMotion, EnemyCharacter->MoveSpeed / 2, false);
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
	ACS330_FinalGameMode* GameMode = Cast<ACS330_FinalGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode->TimeStopped) {
		HandleCurrentState(CurrentState);
	}
}
