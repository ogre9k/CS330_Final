// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerProjectile.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

void APlayerProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor != NULL && OtherActor != this && OtherActor != UGameplayStatics::GetPlayerPawn(GetWorld(), 0) && !Cast<ACS330_FinalProjectile>(OtherActor))
	{
		if (Cast<AEnemyCharacter>(OtherActor))
			OtherActor->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation());
		Destroy();
	}

}