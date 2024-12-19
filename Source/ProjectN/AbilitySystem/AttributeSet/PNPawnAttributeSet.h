// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSet/PNAttributeSet.h"
#include "PNPawnAttributeSet.generated.h"

DECLARE_MULTICAST_DELEGATE(FOutOfHealthDelegate);

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNPawnAttributeSet : public UPNAttributeSet
{
	GENERATED_BODY()
	
private:
	UPNPawnAttributeSet();
	
public:
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, Damage);
	
	mutable FOutOfHealthDelegate OnOutOfHealth;
	
private:
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override final;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override final;
	
private:
	UPROPERTY()
	FGameplayAttributeData Health;

	UPROPERTY()
	FGameplayAttributeData MaxHealth;

	UPROPERTY()
	FGameplayAttributeData Damage;
	
	bool bOutOfHealth = false;
};
