// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CS330_FinalGameMode.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RoomID = 0;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
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

void AEnemyCharacter::Kill()
{
	Destroy();
}

void AEnemyCharacter::UpdateFacing()
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