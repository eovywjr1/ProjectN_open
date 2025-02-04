// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/PNPlayerAttributeSet.h"

#include "GameplayEffectExtension.h"

const float DefaultMaxSR = 6.0f;

UPNPlayerAttributeSet::UPNPlayerAttributeSet()
	: RegenerationHpRate(0.01f),
	  MaxSR(DefaultMaxSR),
	  SR(MaxSR)
{}

void UPNPlayerAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetSRAttribute())
	{
		SetSR(FMath::Clamp(GetSR(), 0.0f, GetMaxSR()));
	}
}
