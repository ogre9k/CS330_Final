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
	_deck.Add(Inferno.Class);
	_deck.Add(Burst.Class);
	_deck.Add(Boomerang.Class);
	_deck.Add(Blitz.Class);
	_deck.Add(Freeze.Class);
	_deck.Add(Inferno.Class);
	_deck.Add(Fireball.Class);
	_deck.Add(Boomerang.Class);


	_hand.Add(_deck[0]);
	_hand.Add(_deck[1]);
	_hand.Add(_deck[4]);

	DeckCounter = _deck.Num() - 1;
}

void ADeckHandler::Shuffle()
{
	;
}

void ADeckHandler::Draw() 
{
	;
}

void ADeckHandler::UseCard(float index)
{
	//Access default variables like this
	//DO NOT CHANGE THEM - They will change for all instances of this object for the current game
	//_hand[index]->GetDefaultObject<ACardEffect>()->ManaCost

	const UWorld* World = GetWorld();
	if (World)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
		if (PlayerCharacter)
		{
			if (_usedCards[index] != true) {
				PlayerCharacter->CardToUse = _hand[index];
				PlayerCharacter->UseCard();
				_usedCards[index] = true;

			}
		}
	}
}
