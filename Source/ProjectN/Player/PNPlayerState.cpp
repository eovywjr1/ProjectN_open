// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PNPlayerState.h"

#include "AbilitySystem/PNAbilitySystemComponent.h"

APNPlayerState::APNPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UPNAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

UAbilitySystemComponent* APNPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
