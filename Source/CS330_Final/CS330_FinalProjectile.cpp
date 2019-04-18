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
	Damage = 1;
	HitForce = 20.0f;
	Color = "Red";
	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

	ProjectileSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileSphere"));
	ProjectileSphere->SetupAttachment(RootComponent);
	ProjectileSphere->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileSphere->OnComponentHit.AddDynamic(this, &ACS330_FinalProjectile::OnHit);
	ProjectileSphere->OnComponentBeginOverlap.AddDynamic(this, &ACS330_FinalProjectile::OnOverlapBegin);
	RootComponent = ProjectileSphere;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileSphere;
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

	// Die after 3 seconds by default
	InitialLifeSpan = 10.0f;
}

// This is used for colliding with the bullet with characters
void ACS330_FinalProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	;
}

// This used for colliding with physical props
void ACS330_FinalProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * HitForce, GetActorLocation());
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation());
	Destroy();
}