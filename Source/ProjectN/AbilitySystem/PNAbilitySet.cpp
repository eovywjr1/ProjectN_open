// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PNAbilitySet.h"

#include "AbilitySystemComponent.h"
#include "PNLogChannels.h"
#include "Abilities/GameplayAbility.h"

void UPNAbilitySet::GiveAbilityToAbilitySystem(UAbilitySystemComponent* InAbilitySystemComponent, UObject* SourceObject) const 
{
	check(InAbilitySystemComponent);
	
	for (int32 AbilityIndex = 0; AbilityIndex < GameAbilities.Num(); ++AbilityIndex)
	{
		const FPNAbilitySet_GameplayAbility& GameAbility = GameAbilities[AbilityIndex];

		if (IsValid(GameAbility.Ability) == false)
		{
			UE_LOG(LogPNAbilitySystem, Error, TEXT("GameAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(GameAbility.Ability);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(GameAbility.InputTag);

		InAbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}
