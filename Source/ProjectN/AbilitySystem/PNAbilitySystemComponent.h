// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PNAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	
	FGameplayEffectSpecHandle MakeOutgoingSpecByGameplayEffect(UGameplayEffect* GameplayEffect, float Level, FGameplayEffectContextHandle Context) const;
	FActiveGameplayEffectHandle ApplyGameplayEffectToSelf(UGameplayEffect* GameplayEffect);
};
