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
	// Sets default values for this character's properties
	AWizardCharacter();


	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
		TSubclassOf<class ACS330_FinalProjectile> Bullet;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser);

	FTimerHandle DeathTimer;
	void UpdateFacing();
	void Kill();

	void SetAggro();


	void FireShot();

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		FVector GunOffset;

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireMin;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireMax;
	float FireRate;
	/* The speed our ship moves around the level */

	FTimerHandle FireTimer;

private:
	bool Aggro = false;
};