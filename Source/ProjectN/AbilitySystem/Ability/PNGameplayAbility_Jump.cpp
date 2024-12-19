// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/PNGameplayAbility_Jump.h"

#include "PNGameplayTags.h"
#include "GameFramework/Character.h"

UPNGameplayAbilityTask_JumpAndWaitForLanding* UPNGameplayAbilityTask_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UPNGameplayAbilityTask_JumpAndWaitForLanding* NewTask = NewAbilityTask<UPNGameplayAbilityTask_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UPNGameplayAbilityTask_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Avatar = CastChecked<ACharacter>(GetAvatarActor());
	Avatar->LandedDelegate.AddDynamic(this, &ThisClass::OnLandedCallback);
	Avatar->Jump();

	SetWaitingOnAvatar();
}

void UPNGameplayAbilityTask_JumpAndWaitForLanding::OnDestroy(bool bInOwnerFinished)
{
	ACharacter* Avatar = CastChecked<ACharacter>(GetAvatarActor());
	Avatar->LandedDelegate.RemoveDynamic(this, &ThisClass::OnLandedCallback);

	Super::OnDestroy(bInOwnerFinished);
}

void UPNGameplayAbilityTask_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}

UPNGameplayAbility_Jump::UPNGameplayAbility_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationOwnedTags.AddTag(FPNGameplayTags::Get().Action_Jump);
}

void UPNGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UPNGameplayAbilityTask_JumpAndWaitForLanding* JumpAndWaitForLandingTask = UPNGameplayAbilityTask_JumpAndWaitForLanding::CreateTask(this);
	JumpAndWaitForLandingTask->OnComplete.AddDynamic(this, &ThisClass::OnLandedCallback);
	JumpAndWaitForLandingTask->ReadyForActivation();
}

void UPNGameplayAbility_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

bool UPNGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) == false)
	{
		return false;
	}

	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	return Character && Character->CanJump();
}

void UPNGameplayAbility_Jump::OnLandedCallback() 
{
	bool bReplicateEndAbility = false;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}