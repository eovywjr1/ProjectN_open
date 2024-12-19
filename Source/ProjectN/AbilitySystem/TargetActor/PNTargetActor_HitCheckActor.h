// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNCommonModule.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "PNTargetActor_HitCheckActor.generated.h"

class UPNWeaponAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class PROJECTN_API APNTargetActor_HitCheckActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	
	FORCEINLINE void SetAttackHitBoxData(const FHitBoxData& InAttackHitBoxData) { AttackHitBoxData = InAttackHitBoxData; };
	
private:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const { return FGameplayAbilityTargetDataHandle(); }
	
public:
	FHitResultDelegate OnComplete;
	
protected:
	FHitBoxData AttackHitBoxData;
};
