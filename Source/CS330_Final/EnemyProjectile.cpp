// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyProjectile.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void AEnemyProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor != NULL && OtherActor != this && !Cast<AEnemyCharacter>(OtherActor) && !Cast<ACS330_FinalProjectile>(OtherActor))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (OtherActor == PlayerCharacter && PlayerCharacter->Color != this->Color) {
			OtherActor->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation());
			Destroy();
		}
	}	
	
}