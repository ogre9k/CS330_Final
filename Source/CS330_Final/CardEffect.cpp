// Fill out your copyright notice in the Description page of Project Settings.

#include "CardEffect.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleEmitter.h"

// Sets default values
ACardEffect::ACardEffect()
{
	Damage = 1;
	Color = "Red";
	ManaCost = 1.0f;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CardImage = NULL;
	
	/*
	this->OnActorBeginOverlap.AddDynamic(this, &ACardEffect::ActorOverlap);
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->BodyInstance.SetCollisionProfileName("CardEffect");
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACardEffect::OnOverlapBegin);
	RootComponent = CollisionBox;
	*/
}

// Called when the game starts or when spawned
void ACardEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACardEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACardEffect::ActorOverlap(AActor* ThisActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Wizard Dead"));
	if (OtherActor != NULL && OtherActor != this && OtherActor != UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		if (Cast<AEnemyCharacter>(OtherActor))
			OtherActor->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation());
	}
}
void ACardEffect::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Wizard Dead"));
	if (OtherActor != NULL && OtherActor != this && OtherActor != UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		if (Cast<AEnemyCharacter>(OtherActor))
			OtherActor->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation());
	}

}



UImage *ACardEffect::GetImage()
{
	return CardImage;
}
