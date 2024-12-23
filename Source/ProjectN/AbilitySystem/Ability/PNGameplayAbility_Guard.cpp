// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/PNGameplayAbility_Guard.h"

#include "PNGameplayTags.h"
#include "PNLogChannels.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"

UPNGameplayAbility_Guard::UPNGameplayAbility_Guard() 
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	const FPNGameplayTags& GameplayTags = FPNGameplayTags::Get();
	
	ActivationOwnedTags.AddTag(GameplayTags.Action_Guard);
	ActivationRequiredTags.AddTag(GameplayTags.Status_Fight);
	ActivationRequiredTags.AddTag(GameplayTags.Action_Idle);
}

void UPNGameplayAbility_Guard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (GuardActionMontage)
	{
		UAbilityTask_PlayMontageAndWait* RollMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Move"), GuardActionMontage);
		RollMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptCallback);
		RollMontageTask->ReadyForActivation();
	}
	else
	{
		UE_LOG(LogPN, Error, TEXT("ActionMontage Not Found in Guard Ability"));
	}
}

void UPNGameplayAbility_Guard::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ACharacter* Avatar = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Avatar->StopAnimMontage(GuardActionMontage);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPNGameplayAbility_Guard::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	bool bReplicateEndAbility = false;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPNGameplayAbility_Guard::OnInterruptCallback() 
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
