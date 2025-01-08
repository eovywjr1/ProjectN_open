// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PNGameplayAbility_Attack.generated.h"

class APNTargetActor_HitCheckActor;

struct FAttackData;

UENUM()
enum class EAttackInvokeInputTimingType : uint8
{
	InputPressed,
	InputReleased
};

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNGameplayAbility_Attack : public UGameplayAbility
{
	GENERATED_BODY()
	
private:
	UPNGameplayAbility_Attack();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override final;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override final;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override final;
	
	UFUNCTION()
	void OnCompleteCallback();
	
	UFUNCTION()
	void OnInterruptedCallback();
	
	UFUNCTION()
	void OnTransitionChargeTimerCallback();
	
private:
	UFUNCTION()
	void OnGameplayEvent(FGameplayEventData Payload);
	
	void ExecuteAttack();
	bool IsEnableExecuteAttack() const;
	void EnableExecuteAttack() const;
	void DisableExecuteAttack() const;
	
	void AttackHitCheck();
	
	UFUNCTION()
	void OnAttackHitTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APNTargetActor_HitCheckActor> TargetActorHitCheckClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	EAttackInvokeInputTimingType AttackInvokeInputTimingType = EAttackInvokeInputTimingType::InputPressed;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "AttackInvokeInputTimingType == EAttackInvokeInputTimingType::InputReleased", EditConditionHides))
	bool bEnableCharge = false;
	
	bool bChargeAttack = false;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bEnableCharge == true", EditConditionHides))
	float ChargeRequirementTime = 0.0f;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag BaseAttackAbilityTag;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bEnableCharge == true", EditConditionHides))
	FGameplayTag ChargeAttackAbilityTag;
	
	const FAttackData* AttackData = nullptr;
};
