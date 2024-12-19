// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/PNWeaponAttributeSet.h"

#include "GameplayTagContainer.h"

UPNWeaponAttributeSet::UPNWeaponAttributeSet()
{
	// Todo. 테스트용도, 추후 데이터테이블에서 가져와야 함
	InitWeaponDamage(10.0f);
}

bool UPNWeaponAttributeSet::GetAttackHitBoxData(FGameplayTag AbilityTag, FHitBoxData& OutAttackHitBoxData) const
{
	if (const FHitBoxData* Data = AttackHitBoxes.Find(AbilityTag))
	{
		OutAttackHitBoxData = *Data;
		return true;
	}

	return false;
}
