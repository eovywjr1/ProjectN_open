// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/PNPawnAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "PNCommonModule.h"
#include "PNGameplayTags.h"
#include "Net/UnrealNetwork.h"

constexpr const uint8 DefaultWalkSpeedDurationPerDefaultMeasurementUnit = 16;
constexpr const uint16 DefaultWalkSpeed = DefaultMeasurementUnit / DefaultWalkSpeedDurationPerDefaultMeasurementUnit;

UPNPawnAttributeSet::UPNPawnAttributeSet()
	: MaxHp(1000.0f),
	  Hp(GetMaxHp()),
	  Power(0.0f),
	  WalkSpeed(DefaultWalkSpeed),
	  RunSpeedMultiplier(2.0f),
	  AttackDamage(0.0f),
	  Damage(0.0f),
	  Heal(0.0f)
{}

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
}

void UPNPawnAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, MaxHp);
	DOREPLIFETIME(ThisClass, Hp);
	DOREPLIFETIME(ThisClass, Power);
	DOREPLIFETIME(ThisClass, WalkSpeed);
	DOREPLIFETIME(ThisClass, RunSpeedMultiplier);
}
