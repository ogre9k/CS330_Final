// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "CS330_FinalProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundBase.h"

const FName APlayerCharacter::MoveForwardBinding("MoveForward");
const FName APlayerCharacter::MoveRightBinding("MoveRight");
const FName APlayerCharacter::FireForwardBinding("FireForward");
const FName APlayerCharacter::FireRightBinding("FireRight");
const FName APlayerCharacter::FireBinding("Fire");

APlayerCharacter::APlayerCharacter()
{
	//set bullet
	static ConstructorHelpers::FClassFinder<ACS330_FinalProjectile> BulletBPClass(TEXT("/Game/Blueprints/Bullets/BP_TestProjectile"));
	if (BulletBPClass.Class != NULL)
	{
		Bullet = BulletBPClass.Class;
	}

	AnimPlaying = false;
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->RelativeRotation = FRotator(-80.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.16f;
	bCanFire = true;

	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}

}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding, this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(MoveRightBinding, this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
	PlayerInputComponent->BindAction(FireBinding, IE_Pressed, this, &APlayerCharacter::OnStartFire);
	PlayerInputComponent->BindAction(FireBinding, IE_Released, this, &APlayerCharacter::OnStopFire);
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	UpdateMouseLook();
}

void APlayerCharacter::OnStartFire()
{
	GetWorldTimerManager().SetTimer(FireTimer, this, &APlayerCharacter::FireShot, FireRate, true, 0.0f);
}

void APlayerCharacter::OnStopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
}
void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
	}
}

void APlayerCharacter::UpdateMouseLook()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			FVector MouseLocation;
			FVector MouseDirection;
			PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);

			MouseDirection *= 100000;
			//MouseDirection
			FHitResult hitResult;
			bool hit;
			hit = World->LineTraceSingleByChannel(hitResult, MouseLocation, MouseLocation + MouseDirection, ECollisionChannel::ECC_Visibility);
			if (hit)
			{
				FVector targetLocation = hitResult.Location;
				FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetLocation);
				this->SetActorRotation(FRotator(0.0f, PlayerRot.Yaw, 0.f));
			}

		}
	}
}
void APlayerCharacter::FireShot()
{
	static FName RHandSocket = FName(TEXT("hand_r"));
	FVector FireDirection = GetActorForwardVector();

	/* This code can be used to spawn at hand
	TArray<USkeletalMeshComponent*> Components;
	GetComponents<USkeletalMeshComponent>(Components);
	for (int32 i = 0; i < Components.Num(); i++)
	{
		USkeletalMeshComponent* SkeletalMeshComponent = Components[i];
	}
	if (Components[0])
	{
		SpawnLocation = Components[0]->GetSocketLocation(RHandSocket);
	}
	*/

	// If it's ok to fire again
	//if (bCanFire == true)
	//{
		if (AnimPlaying)
		{
			AnimPlaying = false;
			PlayAnimMontage(AttackAnim2, 3.0f);
		}
		else
		{
			AnimPlaying = true;
			PlayAnimMontage(AttackAnim1, 3.0f);
		}

		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				// spawn the projectile
				World->SpawnActor<ACS330_FinalProjectile>(Bullet, SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &APlayerCharacter::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	//}
}

void APlayerCharacter::ShotTimerExpired()
{
	bCanFire = true;
}

