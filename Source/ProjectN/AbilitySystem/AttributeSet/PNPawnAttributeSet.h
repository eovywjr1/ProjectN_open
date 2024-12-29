// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSet/PNAttributeSet.h"
#include "PNPawnAttributeSet.generated.h"

DECLARE_MULTICAST_DELEGATE(FOutOfHpDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedPawnAttributeDelegate, FGameplayAttribute);

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNPawnAttributeSet : public UPNAttributeSet
{
	GENERATED_BODY()
	
protected:
	UPNPawnAttributeSet();
	
public:
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, Hp);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, MaxHp);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, Power);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, Heal);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, WalkSpeed);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, RunSpeedMultiplier);
	
	mutable FOutOfHpDelegate OnOutOfHp;
	mutable FOnChangedPawnAttributeDelegate OnChangedPawnAttributeDelegate;
	
protected:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
	
private:
	UPROPERTY()
	FGameplayAttributeData Hp;

	UPROPERTY()
	FGameplayAttributeData MaxHp;

	UPROPERTY()
	FGameplayAttributeData Damage;
	
	UPROPERTY()
	FGameplayAttributeData Power;

	UPROPERTY()
	FGameplayAttributeData Heal;
	
	UPROPERTY()
	FGameplayAttributeData WalkSpeed;
	
	UPROPERTY()
	FGameplayAttributeData RunSpeedMultiplier;
	
	bool bOutOfHp = false;
};
