// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "CardEffect.generated.h"

UCLASS()
class CS330_FINAL_API ACardEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACardEffect();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* CollisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Stats")
		FString Color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Stats")
		bool Shot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		UParticleSystem* HitEffect;
	UFUNCTION(BlueprintCallable)
		virtual	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual	void ActorOverlap(AActor* ThisActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ManaCost;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
