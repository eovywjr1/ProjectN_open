// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PNCharacterMonster.h"

#include "AI/PNAIController.h"
#include "Component/PNActorExtensionComponent.h"

APNCharacterMonster::APNCharacterMonster()
{
	AIControllerClass = APNAIController::StaticClass();
}

void APNCharacterMonster::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
	CreateActorComponent(EActorType::Monster);
}
