// Fill out your copyright notice in the Description page of Project Settings.

#include "BossCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"

ABossCharacter::ABossCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


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