// Fill out your copyright notice in the Description page of Project Settings.


#include "PNActorComponentCreatorInterface.h"

#include "PNActorExtensionComponent.h"
#include "PNDetectComponent.h"
#include "PNEquipmentComponent.h"
#include "PNInteractionComponent.h"
#include "PNInventoryComponent.h"
#include "PNPawnSensingComponent.h"
#include "PNPlayerInputComponent.h"
#include "PNSkillComponent.h"
#include "PNStatusActorComponent.h"

void IPNActorComponentCreatorInterface::CreateActorComponent(EActorType ActorType)
{
	AActor* SelfActor = Cast<AActor>(this);
	check(SelfActor);

	UPNActorExtensionComponent* ActorExtensionComponent = NewObject<UPNActorExtensionComponent>(SelfActor, TEXT("ActorExtensionComponent"));
	ActorExtensionComponent->ActorType = ActorType;
	ActorExtensionComponent->RegisterComponent();

	UPNInteractionComponent* InteractionComponent = NewObject<UPNInteractionComponent>(SelfActor, TEXT("InteractionComponent"));
	InteractionComponent->RegisterComponent();

	if (ActorType >= EActorType::Monster)
	{
		if (IsServerActor(SelfActor))
		{
			UPNSkillComponent* SkillComponent = NewObject<UPNSkillComponent>(SelfActor, TEXT("SkillComponent"));
			SkillComponent->RegisterComponent();

			UPNStatusActorComponent* StatusComponent = NewObject<UPNStatusActorComponent>(SelfActor, TEXT("StatusComponent"));
			StatusComponent->RegisterComponent();
		}

		UPNPawnSensingComponent* SensingComponent = NewObject<UPNPawnSensingComponent>(SelfActor, TEXT("SensingComponent"));
		SensingComponent->RegisterComponent();
	}

	if (ActorType == EActorType::Monster)
	{
		if (IsServerActor(SelfActor))
		{
			UPNDetectComponent* DetectComponent = NewObject<UPNDetectComponent>(SelfActor, TEXT("DetectComponent"));
			DetectComponent->RegisterComponent();
		}
	}

	if (ActorType == EActorType::Player)
	{
		if (IsServerActor(SelfActor))
		{
			UPNEquipmentComponent* EquipmentComponent = NewObject<UPNEquipmentComponent>(SelfActor, TEXT("EquipmentComponent"));
			EquipmentComponent->RegisterComponent();

			UPNInventoryComponent* InventoryComponent = NewObject<UPNInventoryComponent>(SelfActor, TEXT("InventoryComponent"));
			InventoryComponent->RegisterComponent();
		}

		if (IsClientActor(SelfActor))
		{
			UPNPlayerInputComponent* PNPlayerInputComponent = NewObject<UPNPlayerInputComponent>(SelfActor, TEXT("PlayerInputComponent"));
			PNPlayerInputComponent->RegisterComponent();
			
			UPNDetectComponent* DetectComponent = NewObject<UPNDetectComponent>(SelfActor, TEXT("DetectComponent"));
			DetectComponent->RegisterComponent();
		}
	}
}
