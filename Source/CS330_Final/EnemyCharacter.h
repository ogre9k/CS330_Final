// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class CS330_FINAL_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float DMG;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		UParticleSystem* DeathEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* DeathAnim;

	FTimerHandle DeathTimer;
	UFUNCTION(BlueprintCallable)
	void UpdateFacing();
	void Kill();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MoveSpeed;
};
