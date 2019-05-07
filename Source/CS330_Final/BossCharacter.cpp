// Fill out your copyright notice in the Description page of Project Settings.

#include "BossCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ABossCharacter::ABossCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Shooting = false;

}

void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABossCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{


		HP -= ActualDamage;
		if (HP <= 0.0f)
		{
			HP = StartingHP;
			ChangePhase();
		}
	}

	return ActualDamage;
}

void ABossCharacter::StartFire()
{
	Shooting = true;
	if (RandomFireRate)
		GetWorldTimerManager().SetTimer(FireTimer, this, &ABossCharacter::FireShot, FMath::RandRange(FireMin, FireMax), true, 0.0f);
	else
		GetWorldTimerManager().SetTimer(FireTimer, this, &ABossCharacter::FireShot, FireRate, true, 0.0f);
}

void ABossCharacter::StopFire()
{
	Shooting = false;
	GetWorldTimerManager().ClearTimer(FireTimer);
}

// This must always be called before shooting after moving or rotating!
void ABossCharacter::UpdateOffsets()
{
	// Set shooting offset locations
	FVector FireDirection = GetActorForwardVector();
	const FRotator FireRotation = FireDirection.Rotation();
	FVector Location = GetActorLocation();

	const UWorld* World = GetWorld();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));

	Front = Location + FireRotation.RotateVector(FVector(90.f, 0.f, 0.f));
	Left = Location + FireRotation.RotateVector(FVector(0.f, -90.f, 0.f));
	Back = Location + FireRotation.RotateVector(FVector(-90.f, 0.f, 0.f));
	Right = Location + FireRotation.RotateVector(FVector(0.f, 90.f, 0.f));
	Center = Location;
	if (PlayerCharacter)
		Player = PlayerCharacter->GetActorLocation();
	else
		Player = FVector(0.f, 0.f, 0.f);

}