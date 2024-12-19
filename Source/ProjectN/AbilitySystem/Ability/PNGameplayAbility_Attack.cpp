// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/PNGameplayAbility_Attack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "PNGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/AttributeSet/PNWeaponAttributeSet.h"
#include "AbilitySystem/TargetActor/PNTargetActor_HitCheckActor.h"
#include "AbilitySystem/Task/PNAbilityTask_TraceToPawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPNGameplayAbility_Attack::UPNGameplayAbility_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	const FGameplayTag AttackTag = FPNGameplayTags::Get().Action_Attack;
	ActivationOwnedTags.AddTag(AttackTag);
	ActivationBlockedTags.AddTag(AttackTag);
}

void UPNGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* AvatarActor = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	AvatarActor->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), AttackActionMontage, 1.0f, GetNextSectionName());
	PlayAttackTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FPNGameplayTags::Get().GameplayEvent_AttackHitCheck);
	WaitEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEvent);
	WaitEventTask->ReadyForActivation();
}

void UPNGameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ACharacter* AvatarActor = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	AvatarActor->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPNGameplayAbility_Attack::OnCompleteCallback()
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPNGameplayAbility_Attack::OnInterruptedCallback()
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPNGameplayAbility_Attack::OnGameplayEvent(FGameplayEventData Payload)
{
	if (Payload.EventTag == FPNGameplayTags::Get().GameplayEvent_AttackHitCheck)
	{
		AttackHitCheck();
	}
}

void UPNGameplayAbility_Attack::AttackHitCheck()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Checked();
	FGameplayTagContainer TagContainer;
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);

	FGameplayTag AttackTag;
	for (const FGameplayTag& GameplayTag : TagContainer)
	{
		if (GameplayTag.MatchesTag(FPNGameplayTags::Get().Action_Attack))
		{
			AttackTag = GameplayTag;
			break;
		}
	}
	
	const UPNWeaponAttributeSet* WeaponAttributeSet = GetAbilitySystemComponentFromActorInfo()->GetSet<UPNWeaponAttributeSet>();
	check(WeaponAttributeSet);
	FHitBoxData AttackHitBoxData;
	WeaponAttributeSet->GetAttackHitBoxData(AttackTag, AttackHitBoxData);

	if (TargetActorHitCheckClass)
	{
		UPNAbilityTask_TraceToPawn* AttackTraceTask = UPNAbilityTask_TraceToPawn::CreateTask(this, TargetActorHitCheckClass, AttackHitBoxData);
		AttackTraceTask->OnComplete.AddUObject(this, &ThisClass::OnAttackHitTraceResultCallback);
		AttackTraceTask->ReadyForActivation();
	}
}

void UPNGameplayAbility_Attack::OnAttackHitTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffectClass);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}
	}
}
