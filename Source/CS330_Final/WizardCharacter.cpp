// Fill out your copyright notice in the Description page of Project Settings.

#include "WizardCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"

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