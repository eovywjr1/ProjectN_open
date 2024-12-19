// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PNCharacterNonPlayer.h"

#include "Component/PNPawnComponent.h"
#include "AbilitySystem/PNAbilitySystemComponent.h"

APNCharacterNonPlayer::APNCharacterNonPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (UPNAbilitySystemComponent* AbilitySystemComponent = CreateDefaultSubobject<UPNAbilitySystemComponent>(TEXT("AbilitySystemComponent")))
	{
		PawnComponent->SetAbilitySystemComponent(AbilitySystemComponent);
	}
}

void APNCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (UPNAbilitySystemComponent* AbilitySystemComponent = PawnComponent->GetAbilitySystemComponent())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}
