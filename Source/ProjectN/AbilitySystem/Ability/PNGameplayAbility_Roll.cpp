// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/PNGameplayAbility_Roll.h"

#include "PNGameplayTags.h"
#include "PNLogChannels.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Component/PNPlayerInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGameplayAbilityTask_RollMove* UGameplayAbilityTask_RollMove::CreateRollProxy(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InDuration, float InDistance)
{
	check(OwningAbility);

	UGameplayAbilityTask_RollMove* MyObj = NewAbilityTask<UGameplayAbilityTask_RollMove>(OwningAbility, TaskInstanceName);
	MyObj->Duration = InDuration;
	MyObj->Distance = InDistance;

	return MyObj;
}

void UGameplayAbilityTask_RollMove::Activate()
{
	Super::Activate();

	bTickingTask = true;
}

void UGameplayAbilityTask_RollMove::TickTask(float DeltaTime)
{
	ACharacter* Avatar = CastChecked<ACharacter>(GetAvatarActor());
	const UPNPlayerInputComponent* PlayerInputComponent = Avatar->FindComponentByClass<UPNPlayerInputComponent>();
	if (PlayerInputComponent == nullptr)
	{
		return;
	}

	const FVector2D LastMovementInput = PlayerInputComponent->GetLastMovementInput();
	FVector RollDirection;

	if (LastMovementInput.IsNearlyZero())
	{
		RollDirection = Avatar->GetActorForwardVector();
	}
	else
	{
		const FRotator Rotation = Avatar->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		RollDirection = (ForwardDirection * LastMovementInput.Y + RightDirection * LastMovementInput.X).GetSafeNormal();
	}
	
	if (UCharacterMovementComponent* MovementComponent = Avatar->GetCharacterMovement())
	{
		MovementComponent->Velocity = RollDirection * (Distance / Duration);
	}
}

UPNGameplayAbility_Roll::UPNGameplayAbility_Roll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationOwnedTags.AddTag(FPNGameplayTags::Get().Action_Roll);
}

void UPNGameplayAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APawn* AvatarActor = CastChecked<APawn>(ActorInfo->AvatarActor.Get());
	UPNPlayerInputComponent* PlayerInputComponent = AvatarActor->FindComponentByClass<UPNPlayerInputComponent>();
	if (PlayerInputComponent == nullptr)
	{
		return;
	}

	PlayerInputComponent->EnableInput(false);

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

	// Todo. RollDuration을 Montage와 같게 할 것인지 논의 필요
	UGameplayAbilityTask_RollMove* RollTask = UGameplayAbilityTask_RollMove::CreateRollProxy(this, TEXT("Roll"), RollDuration, RollDistance);
	RollTask->ReadyForActivation();
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

	PlayerInputComponent->EnableInput(true);
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