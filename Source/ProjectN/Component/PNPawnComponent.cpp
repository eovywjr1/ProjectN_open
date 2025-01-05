// Fill out your copyright notice in the Description page of Project Settings.


#include "PNPawnComponent.h"

#include "PNPawnData.h"
#include "AbilitySystem/PNAbilitySet.h"
#include "AbilitySystem/PNAbilitySystemComponent.h"

UPNPawnComponent::UPNPawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	PawnData = nullptr;
	AbilitySystemComponent = nullptr;
}

void UPNPawnComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AActor* Owner = GetOwner();
		AbilitySystemComponent->InitAbilityActorInfo(Owner, Owner);

		if (PawnData)
		{
			for (const UPNAbilitySet* AbilitySet : PawnData->AbilitySets)
			{
				if (AbilitySet)
				{
					AbilitySet->GiveAbilityToAbilitySystem(AbilitySystemComponent, this);
				}
			}
		}
	}
}
