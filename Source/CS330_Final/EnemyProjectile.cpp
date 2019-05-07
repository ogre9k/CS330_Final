// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyProjectile.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "CardEffect.h"
#include "Kismet/GameplayStatics.h"

void AEnemyProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor != NULL && OtherActor != this && !Cast<AEnemyCharacter>(OtherActor))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (OtherActor == PlayerCharacter && PlayerCharacter->Color != this->Color && PlayerCharacter->bCanBeDamaged) {
			OtherActor->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation());
			Destroy();
		}
		if (Cast<ACardEffect>(OtherActor))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation());
			Destroy();
		}
	}	
	
}

// This must always be called before shooting after moving or rotating!
void AEnemyProjectile::UpdateOffsets()
{
	// Set shooting offset locations
	FVector FireDirection = GetActorForwardVector();
	const FRotator FireRotation = FireDirection.Rotation();
	FVector Location = GetActorLocation();

	const UWorld* World = GetWorld();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));

	Front = Location + FireRotation.RotateVector(FVector(90.f, 0.f, 0.f));
	Left = Location + FireRotation.RotateVector(FVector(0.f, -90.f, 0.f));
	Back = Location + FireRotation.RotateVector(FVector(-90.f, 0.f, 0.f));
	Right = Location + FireRotation.RotateVector(FVector(0.f, 90.f, 0.f));
	Center = Location;
	if(PlayerCharacter)
		Player = PlayerCharacter->GetActorLocation();
	else
		Player = FVector(0.f, 0.f, 0.f);

}