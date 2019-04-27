// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectile.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "ProjectileEmitter.generated.h"

UCLASS()
class CS330_FINAL_API AProjectileEmitter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileEmitter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		UArrowComponent* DirectionArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting)
		float FireRate;
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

	UFUNCTION(BlueprintCallable)
		void StartFire();
	UFUNCTION(BlueprintCallable)
		void StopFire();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void FireShot();
	FTimerHandle FireTimer;

	UFUNCTION(BlueprintCallable)
		void UpdateOffsets();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
