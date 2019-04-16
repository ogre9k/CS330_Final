// Copyright 1998-2018 Epic Games, Inc. All Rights Reserve

#include "CS330_FinalProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Particles/ParticleEmitter.h"
#include "WizardCharacter.h"
#include "Engine/StaticMesh.h"

ACS330_FinalProjectile::ACS330_FinalProjectile() 
{
	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

	ProjectileSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileSphere"));
	ProjectileSphere->SetupAttachment(RootComponent);
	ProjectileSphere->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileSphere->OnComponentHit.AddDynamic(this, &ACS330_FinalProjectile::OnHit);
	RootComponent = ProjectileSphere;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileSphere;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ACS330_FinalProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherActor != UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		if(OtherComp != NULL && OtherComp->IsSimulatingPhysics())
			OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
		
		OtherActor = Cast<AWizardCharacter>(OtherActor);
		if (OtherActor)
			OtherActor->TakeDamage(10, FDamageEvent(), GetInstigatorController(), this);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation());
		Destroy();
	}
}