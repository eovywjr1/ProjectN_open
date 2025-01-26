// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PNCharacterNonPlayer.h"

#include "Component/PNActorExtensionComponent.h"

void APNCharacterNonPlayer::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
	CreateActorComponent(EActorType::NPC);
}