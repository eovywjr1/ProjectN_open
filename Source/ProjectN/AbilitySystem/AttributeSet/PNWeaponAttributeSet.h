// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNCommonModule.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AttributeSet/PNAttributeSet.h"
#include "PNWeaponAttributeSet.generated.h"

USTRUCT()
struct FAttackData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AttackTag;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> AttackActionMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FName AttackActionMontageSectionName = NAME_None;
};

USTRUCT()
struct FComboData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAttackData> ComboAttackDatas;
};

/**
 * 
 */

// Todo. WeaponData 혹은 EquipmentData로 변경해야 함
UCLASS()
class PROJECTN_API UPNWeaponAttributeSet : public UPNAttributeSet
{
	GENERATED_BODY()

public:
	UPNWeaponAttributeSet();

	ATTRIBUTE_ACCESSORS(UPNWeaponAttributeSet, WeaponDamage);

	bool GetAttackHitBoxData(FGameplayTag AbilityTag, FHitBoxData& OutAttackHitBoxData) const;
	FORCEINLINE TArray<FComboData>::TConstIterator GetComboDatas() const { return ComboDatas.CreateConstIterator(); }

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FHitBoxData> AttackHitBoxes;

	UPROPERTY()
	FGameplayAttributeData WeaponDamage;

	UPROPERTY(EditDefaultsOnly)
	TArray<FComboData> ComboDatas;
};
