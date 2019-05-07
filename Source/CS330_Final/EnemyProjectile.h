// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS330_FinalProjectile.h"
#include "EnemyProjectile.generated.h"

/**
 * 
 */
UCLASS()
class CS330_FINAL_API AEnemyProjectile : public ACS330_FinalProjectile
{
	GENERATED_BODY()
	
public:
	virtual	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void FireShot();

	UFUNCTION(BlueprintCallable)
		void UpdateOffsets();
};
