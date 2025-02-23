// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PNCharacterMonster.h"

#include "Component/PNActorExtensionComponent.h"

void APNCharacterMonster::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
	CreateActorComponent(EActorType::Monster);
}