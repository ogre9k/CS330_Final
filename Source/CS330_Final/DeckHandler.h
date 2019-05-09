// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardEffect.h"
#include "GameFramework/Info.h"
#include "DeckHandler.generated.h"

/**
 * 
 */
UCLASS()
class CS330_FINAL_API ADeckHandler : public AInfo
{
	GENERATED_BODY()
public:
	ADeckHandler();
	UFUNCTION(BlueprintCallable)
		void UseCard(int index);
	int DeckCounter;

	TArray<TSubclassOf<class ACardEffect>> _deck;
	TArray<TSubclassOf<class ACardEffect>> _hand;
	TArray<TSubclassOf<class ACardEffect>> _discard;
	TArray<TSubclassOf<class ACardEffect>> _notInPlay;
	TArray<bool> _usedCards;

	void Shuffle();
	void Draw();
	void ShuffleNotInPlay();
	UFUNCTION(BlueprintCallable)
	void DiscardHand();
	UFUNCTION(BlueprintCallable)
	void AddToDeck();

	// Card Effects
	//Cantrips
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effects")
		TSubclassOf<class ACardEffect> Ember;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effects")
		TSubclassOf<class ACardEffect> Droplet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effects")
		TSubclassOf<class ACardEffect> Breeze;

	//Attacks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effects")
		TSubclassOf<class ACardEffect> Fireball;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effects")
		TSubclassOf<class ACardEffect> Burst;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effects")
		TSubclassOf<class ACardEffect> Boomerang;

	//Buffs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effects")
		TSubclassOf<class ACardEffect> Overheat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effects")
		TSubclassOf<class ACardEffect> Deluge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effects")
		TSubclassOf<class ACardEffect> Blitz;

	//Ultimates
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effects")
		TSubclassOf<class ACardEffect> Inferno;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effects")
		TSubclassOf<class ACardEffect> Freeze;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effects")
		TSubclassOf<class ACardEffect> Tornado;
};
