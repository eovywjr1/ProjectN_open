// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNCommonModule.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AttributeSet/PNAttributeSet.h"
#include "PNWeaponAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNWeaponAttributeSet : public UPNAttributeSet
{
	GENERATED_BODY()
	
public:
	UPNWeaponAttributeSet();
	
	ATTRIBUTE_ACCESSORS(UPNWeaponAttributeSet, WeaponDamage);
	
	bool GetAttackHitBoxData(FGameplayTag AbilityTag, FHitBoxData& OutAttackHitBoxData) const;

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FHitBoxData> AttackHitBoxes;
	
	UPROPERTY()
	FGameplayAttributeData WeaponDamage;
};
