// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PNAbilitySystemComponent.h"

void UPNAbilitySystemComponent::AbilityInputPressed(FGameplayTag InputTag)
{
	if (InputTag.IsValid() == false)
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		AbilitySpec.InputPressed = true;
		
		if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (AbilitySpec.IsActive())
			{
				AbilitySpecInputPressed(AbilitySpec);
			}
			else
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UPNAbilitySystemComponent::AbilityInputReleased(FGameplayTag InputTag) 
{
	if (InputTag.IsValid() == false)
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		AbilitySpec.InputPressed = false;
		
		if (AbilitySpec.IsActive() && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

FGameplayEffectSpecHandle UPNAbilitySystemComponent::MakeOutgoingSpecByGameplayEffect(UGameplayEffect* GameplayEffect, float Level, FGameplayEffectContextHandle Context) const
{
	if (Context.IsValid() == false)
	{
		Context = MakeEffectContext();
	}

	if (GameplayEffect)
	{
		FGameplayEffectSpec* NewSpec = new FGameplayEffectSpec(GameplayEffect, Context, Level);
		return FGameplayEffectSpecHandle(NewSpec);
	}

	return FGameplayEffectSpecHandle(nullptr);
}
