// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/PNPawnAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "PNGameplayTags.h"

UPNPawnAttributeSet::UPNPawnAttributeSet()
	: MaxHealth(100.0f)
{
	InitHealth(GetMaxHealth());
}

bool UPNPawnAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	return true;
}

void UPNPawnAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0.0f, GetMaxHealth()));
		SetDamage(0.0f);
	}

	if (GetHealth() <= 0.0f && bOutOfHealth == false)
	{
		bOutOfHealth = true;
		
		Data.Target.AddLooseGameplayTag(FPNGameplayTags::Get().Status_Dead);
		OnOutOfHealth.Broadcast();
	}
}
