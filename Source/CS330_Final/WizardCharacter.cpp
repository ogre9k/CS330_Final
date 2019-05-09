// Fill out your copyright notice in the Description page of Project Settings.

#include "WizardCharacter.h"
#include "CS330_FinalProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "PlayerCharacter.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "CS330_FinalGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectileEnemyController.h"


AWizardCharacter::AWizardCharacter()
{
	// Set up AI Controller Class
	AIControllerClass = AProjectileEnemyController::StaticClass();

	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireMin = 0.5f;
	FireMax = 1.f;

}
void AWizardCharacter::Tick(float DeltaSeconds)
{
	if (Aggro)
		UpdateFacing();
	
}

void AWizardCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AWizardCharacter::FireShot()
{
	ACS330_FinalGameMode* GameMode = Cast<ACS330_FinalGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		if (!GameMode->TimeStopped)
		{

			FVector FireDirection = GetActorForwardVector();


			const FRotator FireRotation = FireDirection.Rotation();

			// Spawn projectile at an offset from this pawn
			FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				// spawn the projectile
				World->SpawnActor<ACS330_FinalProjectile>(Bullet, SpawnLocation, FireRotation, SpawnParams);
			}
		}
	}
}

float AWizardCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{


		HP -= ActualDamage;
		if (HP <= 0.0f)
		{
			//dead
			bCanBeDamaged = false;
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation());
			//float DeathCountdown = PlayAnimMontage(DeathAnim);
			//GetWorldTimerManager().SetTimer(DeathTimer, this, &AWizardCharacter::Kill, DeathCountdown, false, DeathCountdown-0.75f);
			//UE_LOG(LogTemp, Warning, TEXT("Wizard Dead"));
			Destroy();
		}
	}

	return ActualDamage;
}

void AWizardCharacter::Kill()
{
	Destroy();
}


void AWizardCharacter::UpdateFacing()
{
	const UWorld* World = GetWorld();
	if (World)
	{	
		ACS330_FinalGameMode* GameMode = Cast<ACS330_FinalGameMode>(GetWorld()->GetAuthGameMode());
		if (!GameMode->TimeStopped)
		{
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
			if (PlayerCharacter)
			{
				FVector targetLocation = PlayerCharacter->GetActorLocation();
				FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetLocation);
				SetActorRotation(FRotator(0.0f, NewRot.Yaw, 0.f));
			}
		}
	}
}

void AWizardCharacter::SetAggro() 
{
	Aggro = true;
	GetWorldTimerManager().SetTimer(FireTimer, this, &AWizardCharacter::FireShot, FMath::RandRange(FireMin, FireMax), true, 0.0f);
}