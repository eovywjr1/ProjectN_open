// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/PNGameplayAbility_ComboAttack.h"

#include "PNGameplayTags.h"

UPNGameplayAbility_ComboAttack::UPNGameplayAbility_ComboAttack()
	: Super()
{}

void UPNGameplayAbility_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CurrentCombo = 0;
	bIsNextInputPressed = false;
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	StartComboTimer();
}

void UPNGameplayAbility_ComboAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	bIsNextInputPressed = true;
}

FName UPNGameplayAbility_ComboAttack::GetNextSectionName()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, MaxComboCount);

	return *FString::Printf(TEXT("%d"), CurrentCombo);
}

void UPNGameplayAbility_ComboAttack::StartComboTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ThisClass::CheckAndStartNextCombo, ComboTimeout, false);
}

void UPNGameplayAbility_ComboAttack::CheckAndStartNextCombo()
{
	ComboTimerHandle.Invalidate();

	if (bIsNextInputPressed == false)
	{
		return;
	}
	
	bIsNextInputPressed = true;
	MontageJumpToSection(GetNextSectionName());
	StartComboTimer();
}
