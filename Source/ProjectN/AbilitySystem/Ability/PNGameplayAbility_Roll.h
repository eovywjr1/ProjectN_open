// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PNGameplayAbility_Roll.generated.h"

class UAnimMontage;

UCLASS()
class UGameplayAbilityTask_RollMove : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UGameplayAbilityTask_RollMove* CreateRollProxy(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InDuration, float InDistance);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override final;

private:
	float Duration;
	float Distance;
};

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNGameplayAbility_Roll : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPNGameplayAbility_Roll();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override final;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override final;
	
private:
	UFUNCTION()
	void OnCompleteCallback();
	
	UFUNCTION()
	void OnInterruptCallback();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Roll, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> RollActionMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Roll, Meta = (AllowPrivateAccess = "true"))
	float RollDuration = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Roll, Meta = (AllowPrivateAccess = "true"))
	float RollDistance = 0.0f;
};