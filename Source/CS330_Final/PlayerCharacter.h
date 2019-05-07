// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CardEFfect.h"
#include "Deck.h"
#include "DeckHandler.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class CS330_FINAL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	ADeckHandler* MyDeck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	FString Color;
	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		FVector GunOffset;

	/* How fast the weapon will fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float FireRate;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MoveSpeed;

	/* Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
		class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
        int MaxMP;

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser);
	// End Actor Interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		int MaxMP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		int MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
		TSubclassOf<class ACardEffect> CardToUse;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UseCard();

	/* Fire a shot in the specified direction */
	void FireShot();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool ComboAnimFlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool Shooting;
	UFUNCTION(BlueprintCallable)
	void UpdateMouseLook();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool UpdateFacing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FRotator LastRotation;
	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	TSubclassOf<class ACS330_FinalProjectile> Bullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	TSubclassOf<class ACS330_FinalProjectile> FireBullet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	TSubclassOf<class ACS330_FinalProjectile> WaterBullet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	TSubclassOf<class ACS330_FinalProjectile> AirBullet;

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;
	static const FName FireBinding;

	UFUNCTION(BlueprintCallable)
	void OnStartFire();
	UFUNCTION(BlueprintCallable)
	void OnStopFire();
	FTimerHandle FireTimer;

	/** Navigate player using WASD */
	void MoveForward(float Value);
	void MoveRight(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackAnim1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackAnim2;

	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

private:
};
