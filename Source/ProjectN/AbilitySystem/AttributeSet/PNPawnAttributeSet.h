// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSet/PNAttributeSet.h"
#include "PNPawnAttributeSet.generated.h"

DECLARE_MULTICAST_DELEGATE(FOutOfHpDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDamagedDelegate);

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNPawnAttributeSet : public UPNAttributeSet
{
	GENERATED_BODY()
	
public:
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, MaxHp);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, Hp);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, Power);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, WalkSpeed);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, RunSpeedMultiplier);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, AttackDamage);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UPNPawnAttributeSet, Heal);
	
	mutable FOutOfHpDelegate OnOutOfHp;
	mutable FOnDamagedDelegate OnDamagedDelegate;
	
protected:
	UPNPawnAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY(Replicated)
	FGameplayAttributeData MaxHp;
	
	UPROPERTY(Replicated)
	FGameplayAttributeData Hp;
	
	UPROPERTY(Replicated)
	FGameplayAttributeData Power;
	
	UPROPERTY(Replicated)
	FGameplayAttributeData WalkSpeed;
	
	UPROPERTY(Replicated)
	FGameplayAttributeData RunSpeedMultiplier;
	
	UPROPERTY()
	FGameplayAttributeData AttackDamage;
	
	UPROPERTY()
	FGameplayAttributeData Damage;
	
	UPROPERTY()
	FGameplayAttributeData Heal;
	
	bool bOutOfHp = false;
};
