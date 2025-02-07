// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/PNPlayerAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

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
		UE_LOG(LogTemp, Log, TEXT("SR Override"));
		SetSR(FMath::Clamp(GetSR(), 0.0f, GetMaxSR()));
	}
}

void UPNPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, RegenerationHpRate);
	DOREPLIFETIME(ThisClass, MaxSR);
	DOREPLIFETIME(ThisClass, SR);
}
