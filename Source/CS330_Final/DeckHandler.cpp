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

	// Add Cantrips to the Deck.
	_deck.Add(Ember.Class);
	_deck.Add(Droplet.Class);
	_deck.Add(Breeze.Class);

	// Add Attacks to the Deck.
	_deck.Add(Fireball.Class);
	_deck.Add(Burst.Class);
	_deck.Add(Boomerang.Class);

	// Add Buffs to the Not-In-Play Deck.
	_notInPlay.Add(Overheat.Class);
	_notInPlay.Add(Deluge.Class);
	_notInPlay.Add(Blitz.Class);

	// Add Ultimates to the Not-In-Play Deck.
	_notInPlay.Add(Inferno.Class);
	_notInPlay.Add(Freeze.Class);
	_notInPlay.Add(Tornado.Class);

	// Add all Card Effects to the Catalog Deck.
	_catalog.Add(Ember.Class);
	_catalog.Add(Droplet.Class);
	_catalog.Add(Breeze.Class);
	_catalog.Add(Fireball.Class);
	_catalog.Add(Burst.Class);
	_catalog.Add(Boomerang.Class);
	_catalog.Add(Overheat.Class);
	_catalog.Add(Deluge.Class);
	_catalog.Add(Blitz.Class);
	_catalog.Add(Inferno.Class);
	_catalog.Add(Freeze.Class);
	_catalog.Add(Tornado.Class);

	// Generate a dynamic seed based on time.
	// (Required because apparently a packaged game will not generate a random stream naturally like in the editor).
	FRandomStream SRand = FRandomStream();
	SRand.Initialize(FDateTime::Now().ToUnixTimestamp());

	// Shuffle the Not-In-Play Deck (adds randomization to future deck additions).
	ShuffleNotInPlay();
	// Shuffle the deck, then draw 3 times.
	Shuffle();
	for (int i = 0; i < 3; i++)
	{
		_hand.Add(_deck[0]);
		_deck.RemoveAt(0, 1, true);
	}

	DeckCounter = _deck.Num() - 1;
}

// Function used primarily between phase switching.
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

void ADeckHandler::UseCard(int index)
{
	//Access default variables like this
	//DO NOT CHANGE THEM - They will change for all instances of this object for the current game
	//_hand[index]->GetDefaultObject<ACardEffect>()->ManaCost

	// Testing functionality.
	/*
	if (_usedCards[0] && _usedCards[1] && _usedCards[2])
	{
		UE_LOG(LogTemp, Warning, TEXT("Hand is empty. Attempting to re-draw."));
		AddToDeck();
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
			int NetMP = PlayerCharacter->MP - _hand[index]->GetDefaultObject<ACardEffect>()->ManaCost;
			if (NetMP >= 0)
			{
				if (_usedCards[index] != true) {
					PlayerCharacter->MP = NetMP;
					UE_LOG(LogTemp, Warning, TEXT("Mana expended! %d mana left."), PlayerCharacter->MP, NetMP);
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
				UE_LOG(LogTemp, Warning, TEXT("Not enough mana to use that card! %d mana left."), PlayerCharacter->MP);
			}
		}
	}
	// }
}

// Function used exclusively to shuffle the Not-In-Play deck once.
// Probably could have just made this a single function and passed in
// the deck-to-be-shuffled by reference, but hey.
void ADeckHandler::ShuffleNotInPlay()
{
	int shuffleIterations = 15;
	for (int i = 0; i < shuffleIterations; i++)
	{
		int j = FMath::FloorToInt(FMath::Rand() % _notInPlay.Num());
		int k = FMath::FloorToInt(FMath::Rand() % _notInPlay.Num());
		_notInPlay.Swap(j, k);
	}
}

void ADeckHandler::DiscardHand()
{
	// Discard the hand.
	for (int i = 0; i < 3; i++)
	{
		// Remove one card from the hand at a time and put it into the discard pile.
		_discard.Add(_hand[0]);
		_hand.RemoveAt(0, 1, true);
	}

	// Repopulate the hand.
	Draw();

	// Reset the usedCards flags.
	for (int i = 0; i < _usedCards.Num(); i++)
	{
		_usedCards[i] = false;
	}

	// Restore the Player's Mana.
	const UWorld* World = GetWorld();
	if (World)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
		if (PlayerCharacter)
		{
			PlayerCharacter->MP = PlayerCharacter->MaxMP;
			UE_LOG(LogTemp, Warning, TEXT("Mana restored! %d mana left."), PlayerCharacter->MP);
		}
	}
}

void ADeckHandler::AddToDeck()
{
	if (_notInPlay.Num() > 0)
	{
		_deck.Add(_notInPlay[0]);
		_notInPlay.RemoveAt(0, 1, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("All cards have been added to the deck!"));
	}
}

// The next 2/4 functions are to be strictly used in tandem with the GlobalDeck.
void ADeckHandler::ClearDecks()
{
	for (int i = 0; i < _deck.Num(); i++)
	{
		_deck.RemoveAt(0, 1, true);
	}
	for (int i = 0; i < _hand.Num(); i++)
	{
		_hand.RemoveAt(0, 1, true);
	}
	for (int i = 0; i < _notInPlay.Num(); i++)
	{
		_deck.RemoveAt(0, 1, true);
	}
}

void ADeckHandler::GetFromCatalog(int DeckIndex, int CatalogIndex)
{
	// Holy switch statements. I'm going to hope this doesn't cause massive overhead.
	// Add to Hand.
	if (DeckIndex == 0) 
	{
		switch (CatalogIndex)
		{
		case 0:
			_hand.Add(_catalog[0]);
			break;
		case 1:
			_hand.Add(_catalog[1]);
			break;
		case 2:
			_hand.Add(_catalog[2]);
			break;
		case 3:
			_hand.Add(_catalog[3]);
			break;
		case 4:
			_hand.Add(_catalog[4]);
			break;
		case 5:
			_hand.Add(_catalog[5]);
			break;
		case 6:
			_hand.Add(_catalog[6]);
			break;
		case 7:
			_hand.Add(_catalog[7]);
			break;
		case 8:
			_hand.Add(_catalog[8]);
			break;
		case 9:
			_hand.Add(_catalog[9]);
			break;
		case 10:
			_hand.Add(_catalog[10]);
			break;
		case 11:
			_hand.Add(_catalog[11]);
			break;
		default:
		}
	}
	// Add to Deck.
	else if (DeckIndex == 1)
	{
		switch (CatalogIndex)
		{
		case 0:
			_deck.Add(_catalog[0]);
			break;
		case 1:
			_deck.Add(_catalog[1]);
			break;
		case 2:
			_deck.Add(_catalog[2]);
			break;
		case 3:
			_deck.Add(_catalog[3]);
			break;
		case 4:
			_deck.Add(_catalog[4]);
			break;
		case 5:
			_deck.Add(_catalog[5]);
			break;
		case 6:
			_deck.Add(_catalog[6]);
			break;
		case 7:
			_deck.Add(_catalog[7]);
			break;
		case 8:
			_deck.Add(_catalog[8]);
			break;
		case 9:
			_deck.Add(_catalog[9]);
			break;
		case 10:
			_deck.Add(_catalog[10]);
			break;
		case 11:
			_deck.Add(_catalog[11]);
			break;
		default:
		}
	}
	// Add to NotInPlay.
	else
	{
		switch (CatalogIndex)
		{
		case 0:
			_notInPlay.Add(_catalog[0]);
			break;
		case 1:
			_notInPlay.Add(_catalog[1]);
			break;
		case 2:
			_notInPlay.Add(_catalog[2]);
			break;
		case 3:
			_notInPlay.Add(_catalog[3]);
			break;
		case 4:
			_notInPlay.Add(_catalog[4]);
			break;
		case 5:
			_notInPlay.Add(_catalog[5]);
			break;
		case 6:
			_notInPlay.Add(_catalog[6]);
			break;
		case 7:
			_notInPlay.Add(_catalog[7]);
			break;
		case 8:
			_notInPlay.Add(_catalog[8]);
			break;
		case 9:
			_notInPlay.Add(_catalog[9]);
			break;
		case 10:
			_notInPlay.Add(_catalog[10]);
			break;
		case 11:
			_notInPlay.Add(_catalog[11]);
			break;
		default:
		}
	}
}