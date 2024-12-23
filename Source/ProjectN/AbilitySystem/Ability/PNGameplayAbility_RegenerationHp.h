// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PNGameplayAbility_RegenerationHp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNGameplayAbility_RegenerationHp : public UGameplayAbility
{
	GENERATED_BODY()
	
private:
	UPNGameplayAbility_RegenerationHp();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override final;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override final;
	
	void ApplyHealthRegeneration();
	void OnTagChanged(const FGameplayTag Tag, int32 Count);
	
private:
	FTimerHandle RegenerationTimerHandle;
	FDelegateHandle FightTagDelegateHandle;
	
	uint8 RegenerationRemainTime;
};
