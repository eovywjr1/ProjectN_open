// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNGameplayAbility_Attack.h"
#include "Abilities/GameplayAbility.h"
#include "PNGameplayAbility_ComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNGameplayAbility_ComboAttack : public UPNGameplayAbility_Attack
{
	GENERATED_BODY()

protected:
	UPNGameplayAbility_ComboAttack();

private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	virtual FName GetNextSectionName() override;
	
	void StartComboTimer();
	void CheckAndStartNextCombo();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = "true"))
	float ComboTimeout = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = "true"))
	uint8 MaxComboCount = 0;
	
	uint8 CurrentCombo = 0;
	bool bIsNextInputPressed = false;
	FTimerHandle ComboTimerHandle;
};
