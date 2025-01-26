// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/PNGameplayAbility_Roll.h"

#include "PNGameplayTags.h"
#include "PNLogChannels.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Component/PNPlayerInputComponent.h"

UPNGameplayAbility_Roll::UPNGameplayAbility_Roll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;

	ActivationOwnedTags.AddTag(FPNGameplayTags::Get().Action_Roll);
}

void UPNGameplayAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UPNPlayerInputComponent* PlayerInputComponent = GetAvatarActorFromActorInfo()->FindComponentByClass<UPNPlayerInputComponent>();
	if (PlayerInputComponent == nullptr)
	{
		return;
	}

	PlayerInputComponent->EnableControlInput(false);

	if (RollActionMontage)
	{
		UAbilityTask_PlayMontageAndWait* RollMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Roll"), RollActionMontage);
		RollMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
		RollMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptCallback);
		RollMontageTask->ReadyForActivation();
	}
	else
	{
		UE_LOG(LogPN, Error, TEXT("ActionMontage Not Found in Roll Ability"));
	}
}

void UPNGameplayAbility_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor == nullptr)
	{
		return;
	}

	UPNPlayerInputComponent* PlayerInputComponent = AvatarActor->FindComponentByClass<UPNPlayerInputComponent>();
	if (PlayerInputComponent == nullptr)
	{
		return;
	}

	PlayerInputComponent->EnableControlInput(true);
}

void UPNGameplayAbility_Roll::OnCompleteCallback()
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPNGameplayAbility_Roll::OnInterruptCallback()
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
