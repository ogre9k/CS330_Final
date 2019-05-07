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
#include "CS330_FinalGameMode.h"
#include "Sound/SoundBase.h"

const FName APlayerCharacter::MoveForwardBinding("MoveForward");
const FName APlayerCharacter::MoveRightBinding("MoveRight");
const FName APlayerCharacter::FireForwardBinding("FireForward");
const FName APlayerCharacter::FireRightBinding("FireRight");
const FName APlayerCharacter::FireBinding("Fire");

APlayerCharacter::APlayerCharacter()
{
	//Fire is default element
	ConstructorHelpers::FClassFinder<ACS330_FinalProjectile> FireBulletBPClass(TEXT("/Game/Blueprints/Bullets/PlayerBullets/BP_PFireShot"));
	if (FireBulletBPClass.Class != NULL)
	{
		FireBullet = FireBulletBPClass.Class;
		Bullet = FireBullet;
	}
	ConstructorHelpers::FClassFinder<ACS330_FinalProjectile> WaterBulletBPClass(TEXT("/Game/Blueprints/Bullets/PlayerBullets/BP_PWaterShot"));
	if (WaterBulletBPClass.Class != NULL)
	{
		WaterBullet = WaterBulletBPClass.Class;
	}
	ConstructorHelpers::FClassFinder<ACS330_FinalProjectile> AirBulletBPClass(TEXT("/Game/Blueprints/Bullets/PlayerBullets/BP_PAirShot"));
	if (AirBulletBPClass.Class != NULL)
	{
		AirBullet = AirBulletBPClass.Class;
	}

	HP = 20;
	MaxMP = 3;
	MP = MaxMP;
	Color = "Red";
	ComboAnimFlag = false;
	Shooting = false;
	UpdateFacing = true;
	LastRotation = GetActorRotation();

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1700.f;
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
	
	//This handles updating facing direction while moving and not firing
	if (!Shooting && UpdateFacing)
	{
		// Find movement direction
		const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
		const float RightValue = GetInputAxisValue(MoveRightBinding);

		// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
		const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

		// Calculate  movement
		const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

		// If non-zero size, rotate
		if (Movement.SizeSquared() > 0.0f)
		{
			const FRotator NewRotation = Movement.Rotation();
			RootComponent->SetRelativeRotation(NewRotation);
		}
		//This will keep getting reset to true as long as we're still moving
		UpdateFacing = false;
	}
	//Face towards the mouse if we're shooting
	else if(Shooting)
	{
		UpdateFacing = false;
		UpdateMouseLook();
	}
	//If we're standing still and not shooting, face the last direction we faced
	else 
	{
		RootComponent->SetRelativeRotation(LastRotation);
	}
	
	LastRotation = GetActorRotation();
}

void APlayerCharacter::OnStartFire()
{
	Shooting = true;
	GetWorldTimerManager().SetTimer(FireTimer, this, &APlayerCharacter::FireShot, FireRate, true, 0.0f);
}

void APlayerCharacter::OnStopFire()
{
	Shooting = false;
	GetWorldTimerManager().ClearTimer(FireTimer);
}
void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		UpdateFacing = true;
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		UpdateFacing = true;
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
				FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetLocation);
				SetActorRotation(FRotator(0.0f, NewRot.Yaw, 0.f));
			}

		}
	}
}
void APlayerCharacter::FireShot()
{
	FVector FireDirection = GetActorForwardVector();

	/* This code can be used to spawn at hand
	   Not planning on using it, but leaving as reference
	   static FName RHandSocket = FName(TEXT("hand_r"));
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

	//This handles swapping between left and right hand animation every time we fire
	float AnimSpeed = 18.75 * FireRate; //18.75 is a constant I determined based off testing
	if (ComboAnimFlag)
	{
		ComboAnimFlag = false;
		PlayAnimMontage(AttackAnim2, AnimSpeed);
	}
	else
	{
		ComboAnimFlag = true;
		PlayAnimMontage(AttackAnim1, AnimSpeed);
	}

	const FRotator FireRotation = FireDirection.Rotation();

	// Spawn projectile at an offset from this pawn
	FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		// spawn the projectile
		World->SpawnActor<ACS330_FinalProjectile>(Bullet, SpawnLocation, FireRotation);
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}



float APlayerCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{


		;
	}

	return ActualDamage;
}