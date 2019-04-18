// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS330_FinalProjectile.h"
#include "PlayerProjectile.generated.h"

/**
 * 
 */
UCLASS()
class CS330_FINAL_API APlayerProjectile : public ACS330_FinalProjectile
{
	GENERATED_BODY()
	
public:
	virtual	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
