// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PNGameplayAbility_Run.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNGameplayAbility_Run : public UGameplayAbility
{
	GENERATED_BODY()
	
private:
	UPNGameplayAbility_Run();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override final;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override final;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override final;
	
	UFUNCTION()
	void OnCompleteCallback();
	
	UFUNCTION()
	void OnInterruptCallback();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Run, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> RunActionMontage = nullptr;
};
