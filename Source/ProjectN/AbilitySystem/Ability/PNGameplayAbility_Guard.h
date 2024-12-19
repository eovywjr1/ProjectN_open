// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PNGameplayAbility_Guard.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNGameplayAbility_Guard : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPNGameplayAbility_Guard();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override final;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override final;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override final;
	
private:
	UFUNCTION()
	void OnInterruptCallback();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Guard, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> GuardActionMontage = nullptr;
};
