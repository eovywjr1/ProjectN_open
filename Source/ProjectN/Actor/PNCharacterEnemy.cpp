// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PNCharacterEnemy.h"

#include "Component/PNActorExtensionComponent.h"

void APNCharacterEnemy::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
	CreateActorComponent(EActorType::Enemy);
}