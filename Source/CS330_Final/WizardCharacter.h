// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "WizardCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CS330_FINAL_API AWizardCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	UParticleSystem* DeathEffect;

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser);

	UPROPERTY(EditAnywhere, Category = "Combat")
	float HP;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DMG;
	FTimerHandle DeathTimer;

	void Kill();
};
