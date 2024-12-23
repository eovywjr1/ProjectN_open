// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/PNGameplayAbility_RegenerationHp.h"

#include "PNGameplayTags.h"
#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PNPawnAttributeSet.h"
#include "AbilitySystem/AttributeSet/PNPlayerAttributeSet.h"

constexpr int RegenerationHpCoolTime = 10.0f;

UPNGameplayAbility_RegenerationHp::UPNGameplayAbility_RegenerationHp()
	: RegenerationRemainTime(30)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FPNGameplayTags::Get().Status_Peace;
	AbilityTriggers.Add(TriggerData);
}

void UPNGameplayAbility_RegenerationHp::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(RegenerationTimerHandle, this, &ThisClass::ApplyHealthRegeneration, RegenerationHpCoolTime, true);

	FightTagDelegateHandle = GetAbilitySystemComponentFromActorInfo_Checked()->RegisterGameplayTagEvent(FPNGameplayTags::Get().Status_Fight, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnTagChanged);
}

void UPNGameplayAbility_RegenerationHp::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetWorld()->GetTimerManager().ClearTimer(RegenerationTimerHandle);

	if (FightTagDelegateHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo_Checked()->UnregisterGameplayTagEvent(FightTagDelegateHandle, FPNGameplayTags::Get().Status_Fight, EGameplayTagEventType::NewOrRemoved);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPNGameplayAbility_RegenerationHp::ApplyHealthRegeneration()
{
	UGameplayEffect* RegenerationHpEffect = NewObject<UGameplayEffect>(this, FName(TEXT("RegenerationHpEffect")));
	RegenerationHpEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo StatusModifierInfo;
	StatusModifierInfo.Attribute = UPNPawnAttributeSet::GetHealAttribute();

	UPNAbilitySystemComponent* AbilitySystemComponent = Cast<UPNAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	check(AbilitySystemComponent);

	const UPNPlayerAttributeSet* PlayerAttributeSet = AbilitySystemComponent->GetSet<UPNPlayerAttributeSet>();
	check(PlayerAttributeSet);

	const float RegenerationHp = PlayerAttributeSet->GetMaxHp() * PlayerAttributeSet->GetRegenerationHpRate();
	StatusModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(RegenerationHp));
	RegenerationHpEffect->Modifiers.Add(StatusModifierInfo);

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpecByGameplayEffect(RegenerationHpEffect, 0, EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	--RegenerationRemainTime;
	if (RegenerationRemainTime == 0)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	}
}

void UPNGameplayAbility_RegenerationHp::OnTagChanged(const FGameplayTag Tag, int32 Count)
{
	if (Tag.MatchesTag(FPNGameplayTags::Get().Status_Fight) && Count > 0)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
	}
}
