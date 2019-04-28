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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ChangePhase(); // called when the boss' hp reaches 0 and it needs to change phases or die
};
