// Fill out your copyright notice in the Description page of Project Settings.

#include "DeckHandler.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "CardEffect.h"
#include "PlayerCharacter.h"

/*
Example for how to add a card to a deck/discard/hand
ConstructorHelpers::FClassFinder<ACardEffect> FireballBPClass(TEXT("/Game/Blueprints/Cards/Buffs/BP_CFireball"));
if (FireballBPClass.Class != NULL)
{
	_hand.Add(FireballBPClass.Class);
}
*/

ADeckHandler::ADeckHandler()
{
	// Set up card effects
	ConstructorHelpers::FClassFinder<ACardEffect> Ember(TEXT("/Game/Blueprints/Cards/Cantrips/BP_CEmber"));
	ConstructorHelpers::FClassFinder<ACardEffect> Droplet(TEXT("/Game/Blueprints/Cards/Cantrips/BP_CDroplet"));
	ConstructorHelpers::FClassFinder<ACardEffect> Breeze(TEXT("/Game/Blueprints/Cards/Cantrips/BP_CBreeze"));
	ConstructorHelpers::FClassFinder<ACardEffect> Fireball(TEXT("/Game/Blueprints/Cards/Attacks/BP_CFireball"));
	ConstructorHelpers::FClassFinder<ACardEffect> Burst(TEXT("/Game/Blueprints/Cards/Attacks/BP_CBurst"));
	ConstructorHelpers::FClassFinder<ACardEffect> Boomerang(TEXT("/Game/Blueprints/Cards/Attacks/BP_CBoomerang"));
	ConstructorHelpers::FClassFinder<ACardEffect> Overheat(TEXT("/Game/Blueprints/Cards/Buffs/BP_COverheat"));
	ConstructorHelpers::FClassFinder<ACardEffect> Deluge(TEXT("/Game/Blueprints/Cards/Buffs/BP_CDeluge"));
	ConstructorHelpers::FClassFinder<ACardEffect> Blitz(TEXT("/Game/Blueprints/Cards/Buffs/BP_CBlitz"));
	ConstructorHelpers::FClassFinder<ACardEffect> Inferno(TEXT("/Game/Blueprints/Cards/Ultimates/BP_CInferno"));
	ConstructorHelpers::FClassFinder<ACardEffect> Freeze(TEXT("/Game/Blueprints/Cards/Ultimates/BP_CFreeze"));
	ConstructorHelpers::FClassFinder<ACardEffect> Tornado(TEXT("/Game/Blueprints/Cards/Ultimates/BP_CTornado"));

	_usedCards.Add(false);
	_usedCards.Add(false);
	_usedCards.Add(false);

	_deck.Add(Fireball.Class);
	_deck.Add(Burst.Class);
	_deck.Add(Boomerang.Class);
	_deck.Add(Blitz.Class);
	_deck.Add(Freeze.Class);
	_deck.Add(Inferno.Class);
	_deck.Add(Fireball.Class);
	_deck.Add(Boomerang.Class);

	// Generate a dynamic seed based on time
	// (Required because apparently a packaged game will not generate a random stream naturally like in the editor)
	FRandomStream SRand = FRandomStream();
	SRand.Initialize(FDateTime::Now().ToUnixTimestamp());

	// Draw from the deck 3 times. Start at a random point in the deck.
	// Increment by 1, looping back to index 0 if the iterator exceeds the deck size.
	int i = FMath::FloorToInt(FMath::Rand() % _deck.Num());
	for (int j = 0; j < 3; j++)
	{
		// Add a card to the hand.
		_hand.Add(_deck[i]);
		// Remove that same card from the deck.
		_deck.RemoveAt(i, 1, true);
		// Increment i through the deck.
		i = (i + 1) % _deck.Num();
	}

	DeckCounter = _deck.Num() - 1;
}

void ADeckHandler::Shuffle()
{
	// Add the discard pile back into the deck.
	for (int i = 0; i < _discard.Num(); i++)
	{
		_deck.Add(_discard[0]);	
		_discard.RemoveAt(0, 1, true);
	}

	// Shuffle the deck.
	int shuffleIterations = 15;
	for (int i = 0; i < shuffleIterations; i++)
	{
		int j = FMath::FloorToInt(FMath::Rand() % _deck.Num());
		int k = FMath::FloorToInt(FMath::Rand() % _deck.Num());
		_deck.Swap(j, k);
	}
}

void ADeckHandler::Draw() 
{
	UE_LOG(LogTemp, Warning, TEXT("Drawing cards..."));
	for (int i = 0; i < 3; i++)
	{
		// If the deck is empty, call the shuffle function.
		if (_deck.Num() == 0)
			Shuffle();
		// Draw the first card of the deck and add it to hand.
		_hand.Add(_deck[0]);
		// Remove the first card from the deck.
		_deck.RemoveAt(0, 1, true);
	}
	UE_LOG(LogTemp, Warning, TEXT("Done drawing. You have %d cards in your hand."), _hand.Num());
}

void ADeckHandler::DiscardHand()
{
	// Discard the hand
	for (int i = 0; i < 3; i++)
	{
		// Remove one card from the hand at a time and put it into the discard pile.
		_discard.Add(_hand[0]);
		_hand.RemoveAt(0, 1, true);
	}

	// Repopulate the hand
	Draw();

	// Reset the usedCards flags
	for (int i = 0; i < _usedCards.Num(); i++)
	{
		_usedCards[i] = false;
	}

	// Restore the Player's Mana
	const UWorld* World = GetWorld();
	if (World)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
		if (PlayerCharacter)
		{
			PlayerCharacter->Mana = PlayerCharacter->MaxMana;
			UE_LOG(LogTemp, Warning, TEXT("Mana restored! %d mana left."), PlayerCharacter->Mana);
		}
	}
}

void ADeckHandler::UseCard(int index)
{
	//Access default variables like this
	//DO NOT CHANGE THEM - They will change for all instances of this object for the current game
	//_hand[index]->GetDefaultObject<ACardEffect>()->ManaCost

	// Testing functionality
	/*
	if (_usedCards[0] && _usedCards[1] && _usedCards[2])
	{
		UE_LOG(LogTemp, Warning, TEXT("Hand is empty. Attempting to re-draw."));
		DiscardHand();
	}
	else
	{
	*/
	const UWorld* World = GetWorld();
	if (World)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
		if (PlayerCharacter)
		{
			int NetMana = PlayerCharacter->Mana - _hand[index]->GetDefaultObject<ACardEffect>()->ManaCost;
			if (NetMana >= 0)
			{
				if (_usedCards[index] != true) {
					PlayerCharacter->Mana = NetMana;
					UE_LOG(LogTemp, Warning, TEXT("Mana expended! %d mana left."), PlayerCharacter->Mana);
					PlayerCharacter->CardToUse = _hand[index];
					PlayerCharacter->UseCard();
					_usedCards[index] = true;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Card was already used. Wait until you draw more cards."));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Not enough mana to use that card! %d mana left."), PlayerCharacter->Mana);
			}
		}
	}
	// }
}