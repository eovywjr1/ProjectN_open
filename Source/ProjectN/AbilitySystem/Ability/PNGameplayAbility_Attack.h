// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PNGameplayAbility_Attack.generated.h"

class APNTargetActor_HitCheckActor;

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNGameplayAbility_Attack : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPNGameplayAbility_Attack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual FName GetNextSectionName() { return NAME_None; }
	
	UFUNCTION()
	void OnCompleteCallback();
	
	UFUNCTION()
	void OnInterruptedCallback();
	
private:
	UFUNCTION()
	void OnGameplayEvent(FGameplayEventData Payload);
	
	void AttackHitCheck();
	
	UFUNCTION()
	void OnAttackHitTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attack)
	TObjectPtr<UAnimMontage> AttackActionMontage = nullptr;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APNTargetActor_HitCheckActor> TargetActorHitCheckClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> AttackDamageEffectClass = nullptr;
};
