// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileEmitter.h"
#include "TimerManager.h"

// Children must add bullets to the bullets array and implement the FireShot event. 
// They also need to control when firing starts and stops.


// Sets default values
AProjectileEmitter::AProjectileEmitter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrow"));
	RootComponent = DirectionArrow;
}

// Called when the game starts or when spawned
void AProjectileEmitter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOffsets();
}

// Called every frame
void AProjectileEmitter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileEmitter::StartFire()
{
	GetWorldTimerManager().SetTimer(FireTimer, this, &AProjectileEmitter::FireShot, FireRate, true, 0.0f);
}

void AProjectileEmitter::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
}

// This must always be called before shooting after moving or rotating!
void AProjectileEmitter::UpdateOffsets()
{
	// Set shooting offset locations
	FVector FireDirection = GetActorForwardVector();
	const FRotator FireRotation = FireDirection.Rotation();
	FVector Location = GetActorLocation();

	Front = Location + FireRotation.RotateVector(FVector(90.f,0.f,0.f));
	Left = Location + FireRotation.RotateVector(FVector(0.f, -90.f, 0.f));
	Back = Location + FireRotation.RotateVector(FVector(-90.f, 0.f, 0.f));
	Right = Location + FireRotation.RotateVector(FVector(0.f, 90.f, 0.f));
	Center = Location;
}