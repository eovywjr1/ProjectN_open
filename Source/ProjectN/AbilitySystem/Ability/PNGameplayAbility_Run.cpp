// Fill out your copyright notice in the Description page of Project Settings.


#include "PNGameplayAbility_Run.h"

#include "PNGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPNGameplayAbility_Run::UPNGameplayAbility_Run()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationOwnedTags.AddTag(FPNGameplayTags::Get().Action_Run);
}

void UPNGameplayAbility_Run::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Avatar = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	UCharacterMovementComponent* MovementComponent = Avatar->GetCharacterMovement();
	if (MovementComponent == nullptr)
	{
		return;
	}

	MovementComponent->MaxWalkSpeed *= SpeedMultiplier;

	if (RunActionMontage)
	{
		UAbilityTask_PlayMontageAndWait* RunMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Run"), RunActionMontage);
		RunMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
		RunMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptCallback);
		RunMontageTask->ReadyForActivation();
	}
}

void UPNGameplayAbility_Run::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ACharacter* Avatar = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	if (UCharacterMovementComponent* MovementComponent = Avatar->GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed /= SpeedMultiplier;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPNGameplayAbility_Run::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPNGameplayAbility_Run::OnCompleteCallback()
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPNGameplayAbility_Run::OnInterruptCallback()
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}