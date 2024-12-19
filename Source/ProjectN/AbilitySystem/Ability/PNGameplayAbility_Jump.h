// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PNGameplayAbility_Jump.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpAndWaitForLandingDelegate);

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNGameplayAbilityTask_JumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	static UPNGameplayAbilityTask_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);

private:
	virtual void Activate() override final;
	virtual void OnDestroy(bool bInOwnerFinished) override final;
	
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);
	
public:
	FJumpAndWaitForLandingDelegate OnComplete;
};

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNGameplayAbility_Jump : public UGameplayAbility
{
	GENERATED_BODY()
	
private:
	UPNGameplayAbility_Jump();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override final;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override final;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override final;
	
	UFUNCTION()
	void OnLandedCallback();
};
