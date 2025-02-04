// Fill out your copyright notice in the Description page of Project Settings.


#include "PNActorComponentCreatorInterface.h"

#include "PNActorExtensionComponent.h"
#include "PNDetectComponent.h"
#include "PNEquipmentComponent.h"
#include "PNInteractionComponent.h"
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

	if (ActorType >= EActorType::Enemy)
	{
		UPNSkillComponent* SkillComponent = NewObject<UPNSkillComponent>(SelfActor, TEXT("SkillComponent"));
		SkillComponent->RegisterComponent();

		UPNDetectComponent* DetectComponent = NewObject<UPNDetectComponent>(SelfActor, TEXT("DetectComponent"));
		DetectComponent->RegisterComponent();

		UPNStatusActorComponent* StatusComponent = NewObject<UPNStatusActorComponent>(SelfActor, TEXT("StatusComponent"));
		StatusComponent->RegisterComponent();
	}
	
	if(ActorType == EActorType::Player)
	{
		UPNEquipmentComponent* EquipmentComponent = NewObject<UPNEquipmentComponent>(SelfActor, TEXT("EquipmentComponent"));
		EquipmentComponent->RegisterComponent();
	}
}
