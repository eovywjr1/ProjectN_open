// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PNCharacterNonPlayer.h"

#include "Component/PNInteractionComponent.h"
#include "Component/PNPawnComponent.h"

APNCharacterNonPlayer::APNCharacterNonPlayer()
{
	CreateDefaultSubobject<UPNInteractionComponent>(TEXT("InteractionComponent"));
	
	PawnComponent->ActorType = EActorType::NPC;
}
