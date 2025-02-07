// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSet/PNPawnAttributeSet.h"
#include "PNPlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNPlayerAttributeSet : public UPNPawnAttributeSet
{
	GENERATED_BODY()
	
public:
	ATTRIBUTE_ACCESSORS(UPNPlayerAttributeSet, RegenerationHpRate);
	ATTRIBUTE_ACCESSORS(UPNPlayerAttributeSet, MaxSR);
	ATTRIBUTE_ACCESSORS(UPNPlayerAttributeSet, SR);
	
private:
	UPNPlayerAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override final;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override final;
	
private:
	UPROPERTY(Replicated)
	FGameplayAttributeData RegenerationHpRate;
	
	UPROPERTY(Replicated)
	FGameplayAttributeData MaxSR;
	
	UPROPERTY(Replicated)
	FGameplayAttributeData SR;
};
