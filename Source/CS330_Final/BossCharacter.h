// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "BossCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CS330_FINAL_API ABossCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	

public:
	ABossCharacter();

	virtual void Tick(float DeltaTime) override;
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
			AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float StartingHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int CurrentPhase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int FinalPhase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* AttackAnim1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* AttackAnim2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		bool ComboAnimFlag;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ChangePhase(); // called when the boss' hp reaches 0 and it needs to change phases or die

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting)
		float FireRate;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireMin;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireMax;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		bool RandomFireRate;
	UPROPERTY(Category = Offsets, EditAnywhere, BlueprintReadWrite)
		bool Shooting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting)
		TArray<TSubclassOf<class AEnemyProjectile>> Bullets;

	// Bullet offset positions
	UPROPERTY(Category = Offsets, EditAnywhere, BlueprintReadWrite)
		FVector Front;
	UPROPERTY(Category = Offsets, EditAnywhere, BlueprintReadWrite)
		FVector Left;
	UPROPERTY(Category = Offsets, EditAnywhere, BlueprintReadWrite)
		FVector Back;
	UPROPERTY(Category = Offsets, EditAnywhere, BlueprintReadWrite)
		FVector Right;
	UPROPERTY(Category = Offsets, EditAnywhere, BlueprintReadWrite)
		FVector Center;
	UPROPERTY(Category = Offsets, EditAnywhere, BlueprintReadWrite)
		FVector Player;

	UFUNCTION(BlueprintCallable)
		void StartFire();
	UFUNCTION(BlueprintCallable)
		void StopFire();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void FireShot();
	FTimerHandle FireTimer;
	UFUNCTION(BlueprintCallable)
		void UpdateOffsets();
};
