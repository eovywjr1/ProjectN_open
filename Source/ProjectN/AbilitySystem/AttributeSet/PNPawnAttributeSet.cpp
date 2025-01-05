// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/PNPawnAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "PNCommonModule.h"
#include "PNGameplayTags.h"

constexpr const uint8 DefaultWalkSpeedDurationPerDefaultMeasurementUnit = 16;
constexpr const uint16 DefaultWalkSpeed = static_cast<uint16>(EPNDistanceUnit::DefaultMeasurementUnit) / DefaultWalkSpeedDurationPerDefaultMeasurementUnit;

UPNPawnAttributeSet::UPNPawnAttributeSet()
	: MaxHp(1000.0f),
	  Damage(0.0f),
	  Power(0.0f),
	  Heal(0.0f),
	  WalkSpeed(DefaultWalkSpeed),
	  RunSpeedMultiplier(2.0f)

{
	InitHp(GetMaxHp());
}

void UPNPawnAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHp(FMath::Clamp(GetHp() - GetDamage(), 0.0f, GetMaxHp()));
		SetDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetPowerAttribute())
	{
		if (GetPower() < 0.0f)
		{
			SetPower(0.0f);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealAttribute())
	{
		if (GetHeal() > 0.0f)
		{
			SetHp(FMath::Clamp(GetHp() + GetHeal(), 0.0f, GetMaxHp()));
		}

		SetHeal(0.0f);
	}

	if (GetHp() <= 0.0f && bOutOfHp == false)
	{
		bOutOfHp = true;

		Data.Target.AddLooseGameplayTag(FPNGameplayTags::Get().Status_Dead);
		OnOutOfHp.Broadcast();
	}

	OnChangedPawnAttributeDelegate.Broadcast(Data.EvaluatedData.Attribute);
}
